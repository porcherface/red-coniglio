# standard libs
import os 
import sys
import argparse
import pygame
import subprocess
from editor.editor import EditorGui
import time

# the infamous appjar library
from appJar import gui

GRID_SIZE = 10
COLOR_DIC = {
    
    0 : "grey" ,
    1 : "red" ,
    2 : "green" ,
    3 : "blue" ,
    4 : "white",
    "grey" : 0,
    "red" : 1, 
    "green" : 2,
    "blue" : 3,
    "white" :4 
}

CMD_DIC = {
    
    0 : "<" ,
    1 : "^" ,
    2 : ">" ,
    3 : "f0" ,
    4 : "f1" ,
    5 : "f2" ,
    6 : "Pr" ,
    7 : "Pg" ,
    8 : "Pb"  
}

CND_DIC = {
        
    0 : "x",
    1 : "r",
    2 : "g",
    3 : "b"
}

# a data structure containing level info
class LevelParameters():
    def __init__(self):
        self.nf0=0
        self.nf1=0
        self.nf2=0

        self.f0 = ""
        self.f1 = ""
        self.f2 = ""

        self.static_map = []
        self.dynamic_map =[]

        self.valid_commands=[]

        self.startposition=""
        self.starspositions=""
        self.starcounter=0

        self.levelname = ""
        self.level_log = ""

#a data structure containing simulation animation data
class Animation():
    def __init__(self):
        
        self.framelist =[]
        self.x=0
        self.y=0
        self.angle=0
        self.currframe =0
        self.has_frames = False


# main class, contains app layout, buttons, logic, calls and so on
class RedConiglio():

    # Build the GUI
    def Prepare(self, app):
        # Form GUI

        self.LEVEL_READY = False
        self.level_params = LevelParameters()
        self.animation = Animation()
        self.has_animation = False

        app.setTitle("RedConiglio")
        #app.setSize("fullscreen")
        app.setSize("1000x800")
        helpMenus=["Help","about...","-","?"]
        app.addMenuList("Help", helpMenus, self.menuPress)
        app.setSticky("news")
        app.setExpand("both")
        app.setFont(16)
        app.setStopFunction(self.BeforeExit)
        #app.after(1,self.AnimateFrame())

        for i in range(GRID_SIZE):
            for j in range(GRID_SIZE):

                name = str(i)+","+str(j)
                app.addLabel(name,name,i,j,1,1)

        msglist = ["Hello REDCONIGLIO!"]

        msglist.append("|---------------------|")
        msglist.append("| НЕМЕЦКИЕ   |")
        msglist.append("| СВИНЬИ         |")
        msglist.append("| ЦИКА              |")
        msglist.append("| БЛЯТЬ            |")
        msglist.append("|---------------------|")
        msglist.append("(\\__/) ||")
        msglist.append("(•ㅅ•) ||")
        msglist.append("/ 　 づ")


        app.addButton("Open Editor",self.goEditor,0,GRID_SIZE+1,1,1)
        app.addButton("Load Level",self.loadLevel,1,GRID_SIZE+1,1,1)
        app.addButton("Exit",self.BeforeExit,0,GRID_SIZE+2,1,1)
        
        app.addListBox("mess",msglist,GRID_SIZE+1,0,GRID_SIZE,1)

        # app.setBgImage("res/coniglio_bg.png");

        app.addButton("<",self.Command,2,GRID_SIZE+1)
        app.addButton("^",self.Command,2,GRID_SIZE+2)
        app.addButton(">",self.Command,2,GRID_SIZE+3)
        
        app.addButton("f0",self.Command,3,GRID_SIZE+1)
        app.addButton("f1",self.Command,3,GRID_SIZE+2)
        app.addButton("f2",self.Command,3,GRID_SIZE+3)

        app.addButton("r",self.Command,4,GRID_SIZE+1)
        app.addButton("g",self.Command,4,GRID_SIZE+2)
        app.addButton("b",self.Command,4,GRID_SIZE+3)
        app.addButton("x",self.Command,4,GRID_SIZE+4)

        app.addButton("Pr",self.Command,5,GRID_SIZE+1)
        app.addButton("Pg",self.Command,5,GRID_SIZE+2)
        app.addButton("Pb",self.Command,5,GRID_SIZE+3)
        
        commandButtonList = ["<","^",">","f0","f1","f2","r","g","b","Pr","Pg","Pb","x"]

        for thisguy in commandButtonList:
            app.disableButton(thisguy)

            
        pygame.mixer.init()
        pygame.mixer.music.load("res/soviet_anthem.wav")
        pygame.mixer.music.play()

        app.setLabelFont(size=26)        
        app.addLabel("cmd:","cmd:",6,GRID_SIZE+1)
        app.addLabel("cmd","  ",6,GRID_SIZE+2)

        app.addLabel("slots: ","slots: ",7,GRID_SIZE+1)
        app.addLabel("num"," ",7,GRID_SIZE+2)
        #app.addEntry("con",6,GRID_SIZE+2)

        #app.setEntryDefault("cmd","cmd")
        #app.setEntryDefault("con","con")
        
        app.addValidationEntry("entry0",7+1,GRID_SIZE+1,3,1)
        app.addValidationEntry("entry1",8+1,GRID_SIZE+1,3,1)
        app.addValidationEntry("entry2",9+1,GRID_SIZE+1,3,1)

        app.addNamedButton("V","+0",self.Command,7+1,GRID_SIZE+4)
        app.addNamedButton("X","-0",self.Command,7+1,GRID_SIZE+5)
        app.addNamedButton("V","+1",self.Command,8+1,GRID_SIZE+4)
        app.addNamedButton("X","-1",self.Command,8+1,GRID_SIZE+5)
        app.addNamedButton("V","+2",self.Command,9+1,GRID_SIZE+4)
        app.addNamedButton("X","-2",self.Command,9+1,GRID_SIZE+5)

        for i in ["0","1","2"]:
            app.disableButton("+"+i)
            app.disableButton("-"+i)
            app.disableEntry("entry"+i)
            app.setEntryDisabledBg("entry"+i,"grey")
            app.setEntryDefault("entry"+i, "f"+i+" commands here")

        app.addImageButton("GO",self.GO, "res/bloodconiglio.gif", 11, GRID_SIZE+1)    
        app.addImageButton("RED",self.RED, "res/redconiglio_icon.gif", 11, GRID_SIZE+2)    

        app.startSubWindow("editor")
        editorApp = EditorGui()
        editorApp.Prepare(app)
        app.stopSubWindow()
        return app
        
    # Build and Start your application
    def Start(self):
        # Creates a UI
        app = gui()

        # Run the prebuild method that adds items to the UI
        app = self.Prepare(app)

        # Make the app class-accesible
        self.app = app

        # Start appJar
        app.go()

    # Callback execute before quitting your app
    def BeforeExit(self):
        sys.exit(0)
        return 1#self.app.yesNoBox("Confirm Exit", "Are you sure you want to exit the application?")
    
    # load the editor app
    def goEditor(self):
        #self.editorApp.CallMe(self.app,"editor")
        print("tryin to open editor")
        self.app.showSubWindow("editor")

    # loads a level into map and sets the data structure
    def loadLevel(self):
        
        self.Refresh()
        

        levelname = self.app.openBox()
        self.level_params.levelname = levelname

        level_lines = open(levelname,"r").readlines()
        self.app.setTitle("RedConiglio - "+levelname.split("/")[-1])
        self.app.addListItem("mess","Loading level: "+levelname.split("/")[-1])
        for idx,line in enumerate(level_lines):
            level_lines[idx] = line.strip("\n")

        #print(levelname+"\n") 
        map_raw = level_lines[3:13]
        stars_raw = level_lines[17]
        start_raw = level_lines[18]
        num_commands_raw = level_lines[13]
        valid_cmd_raw = level_lines[15:17]
        valid_cmd_human_form = level_lines[19]

        print(map_raw)
        print(stars_raw)
        print(start_raw)
        print(num_commands_raw)
        print(valid_cmd_raw)
        self.app.addListItem("mess","Enabling commands...")
        for i in range(10):
            for j in range(10):
                row = map_raw[i]
                self.level_params.static_map.append(row)
                item = row.split(" ")[j]
                value = int(item)%10
                labelname=str(i)+","+str(j)
                self.app.setLabelBg(labelname, COLOR_DIC[value])
                self.app.addImage("happyface"+str(i)+","+str(j),"res/happyface.gif",i,j)
                self.app.hideImage("happyface"+str(i)+","+str(j))
                self.app.setImageBg("happyface"+str(i)+","+str(j),self.app.getLabelBg(labelname))

        if(num_commands_raw.split(" ")[0] != '0'):        
            self.app.enableEntry("entry0")
            self.app.enableButton("+0")
            self.app.enableButton("-0")
            self.level_params.nf0=int(num_commands_raw.split(" ")[0])
        if(num_commands_raw.split(" ")[1] != '0'):        
            self.app.enableEntry("entry1")
            self.app.enableButton("+1")
            self.app.enableButton("-1")
            self.level_params.nf1=int(num_commands_raw.split(" ")[1])
        if(num_commands_raw.split(" ")[2] != '0'):        
            self.app.enableEntry("entry2")
            self.app.enableButton("+2")
            self.app.enableButton("-2")
            self.level_params.nf2=int(num_commands_raw.split(" ")[2])
        

        self.level_params.startposition =start_raw.split("=")[1][1:-1]
        
        x = int(self.level_params.startposition[0])
        y = int(self.level_params.startposition[2])

        self.level_params.starcounter =0
        self.level_params.starspositions =stars_raw.split("=(")[1].split("),(")
        for posx in self.level_params.starspositions:
            pos=posx.strip("(").strip(")")
            xs=int(pos[0])
            ys=int(pos[2])
            self.level_params.starcounter+=1
            #self.app.addImage("happyheart"+str(self.level_params.starcounter),"res/happyheart.gif",xs,ys)
            #self.app.setImageBg("happyheart"+str(self.level_params.starcounter),self.app.getLabelBg(pos))
            self.app.addImage("happyheart"+pos,"res/happyheart.gif",xs,ys)
            self.app.setImageBg("happyheart"+pos,self.app.getLabelBg(pos))
            
        self.app.showImage("happyface"+str(x)+","+str(y))
        self.app.setImageBg("happyface"+str(x)+","+str(y),self.app.getLabelBg(self.level_params.startposition[0:3]))

        cmdlist=[]
        conlist=[]

        for item in valid_cmd_raw[0].split(" ")[:-1]:
            buttonName = CMD_DIC[int(item)]
            print("enabling("+buttonName+")")
            self.app.enableButton(buttonName)
            cmdlist.append(buttonName.strip("f").replace("Pr","R").replace("Pb","B").replace("Pg","G"))

        for item in valid_cmd_raw[1].split(" ")[1:-1]:
            buttonName = CND_DIC[int(item)]
            print("enabling("+buttonName+")")
            self.app.enableButton(buttonName)
            conlist.append(buttonName)

        self.level_params.valid_commands=[cmdlist,conlist]
        self.app.enableButton("x")    

        self.LEVEL_READY = True
        self.app.addListItem("mess","READY!")

        #sys.exit(0)
    
    # this is called every time a button is pushed
    def Command(self,name):
        #print("asked for button "+name)
        old_cmd = self.app.getLabel("cmd")

        if name in ["<","^",">","f0","f1","f2","Pr","Pg","Pb"]:
            old_cmd=old_cmd[0]+name
        if name in ["x","b","g","r"]:
            if name == "x":
                name = " "
            old_cmd=name+old_cmd[1:]

        self.app.setLabel("cmd",old_cmd)

        if name in ["+0","+1","+2"]:
            old_val=self.app.getEntry("entry"+name[1])
            new_val=old_val+" "+self.app.getLabel("cmd")
            self.app.setEntry("entry"+name[1],new_val)

        if name in ["-0","-1","-2"]:
            self.app.setEntry("entry"+name[1],"")    

        valid = self.check_entries()  

        if(valid == False):
            self.app.disableButton("GO")
        else:
            self.app.enableButton("GO")

    # a check on inputs submitted
    def check_entries(self):
        
        #get number
        N0 = self.level_params.nf0
        N1 = self.level_params.nf1
        N2 = self.level_params.nf2
                

        entries0 = self.app.getEntry("entry0").replace("  "," ").split(" ")
        entries1 = self.app.getEntry("entry1").replace("  "," ").split(" ")
        entries2 = self.app.getEntry("entry2").replace("  "," ").split(" ")
        
        num0 =len( entries0)-1
        num1 =len( entries1)-1
        num2 =len( entries2)-1

        slotrecap=str(num0)+"["+str(N0)+"],"+str(num1)+"["+str(N1)+"],"+str(num2)+"["+str(N2)+"]"
        self.app.setLabel("num",slotrecap)
        valid=True

        self.app.setEntryWaitingValidation("entry0")
        self.app.setEntryWaitingValidation("entry1")
        self.app.setEntryWaitingValidation("entry2")

        if(num0>N0):
            self.app.setEntryInvalid("entry0")
            valid=False
        if(num1>N1):
            self.app.setEntryInvalid("entry1")
            valid=False
        if(num2>N2):
            self.app.setEntryInvalid("entry2")
            valid=False

        in0 = self.app.getEntry("entry0").replace("f","").strip(",")
        in1 = self.app.getEntry("entry1").replace("f","").strip(",")
        in2 = self.app.getEntry("entry2").replace("f","").strip(",")

        cmdlist =  self.level_params.valid_commands[0]
        conlist =  self.level_params.valid_commands[1]

        instruction= (in0+" ")*(num0>0)+(in1+" ")*(num1>0)+in2*(num2>0)
        if instruction is None or instruction == "":
            return True

        print(instruction)
        instruction2 = instruction.replace("  "," ").split(" ")

        print(instruction2)
        for idx,thisguy in enumerate(instruction2):
            if thisguy != "":
                if thisguy[-1] not in cmdlist:
                    self.app.setEntryInvalid("entry0")
                    self.app.setEntryInvalid("entry1")
                    self.app.setEntryInvalid("entry2")
                    return False
                if thisguy[0] not in cmdlist and thisguy[0] not in conlist:
                    self.app.setEntryInvalid("entry0")
                    self.app.setEntryInvalid("entry1")
                    self.app.setEntryInvalid("entry2")
                    return False

        if(num0==N0):
            self.app.setEntryValid("entry0")
        if(num1==N1):
            self.app.setEntryValid("entry1")
        if(num2==N2):
            self.app.setEntryValid("entry2")

        return valid

    # calls the simulator
    def GO(self):

        if self.check_entries() is False:
            return

        in0 = self.app.getEntry("entry0").replace("f","")
        in1 = self.app.getEntry("entry1").replace("f","")
        in2 = self.app.getEntry("entry2").replace("f","")

        instruction= (in0+" "+in1+" "+in2);
        command_line=["./engine/minigame.out",self.level_params.levelname,instruction]
        #subprocess.run(["./../../bin/minigame.out "+self.level_params.levelname],shell=True)
        #ls_lines = subprocess.check_output(command_line).splitlines()
        #print(ls_lines)
        process_result = subprocess.run(command_line,stdout=subprocess.PIPE)
        
        #print(process_result.stdout.readlines())
        string_out = str(process_result.stdout.decode('ascii'))
        
        output_list = string_out.splitlines()
        print(output_list)
        self.app.addListItems("mess",output_list[-5:])
        self.app.addListItem("mess",process_result.returncode)
        
        self.level_params.level_log = output_list
        
        # ANIMATION ON GO COMMAND
        self.Animate()
        
    # CALLS THE OPTIMIZATION ENGINE
    def RED_CALL(self):
        command_line=["./engine/redconiglio.out",self.level_params.levelname]
        self.app.addListItem("mess","!!! LAUNCHING REDCONIGLIO ENGINE !!!")
        self.app.addListItem("mess","cmd to console: "+command_line[0]+" "+command_line[1])
        #subprocess.run(["./../../bin/redconiglio.out "+self.level_params.levelname],shell=True)
        process_result =subprocess.run(command_line,stdout=subprocess.PIPE)
        
        #print(process_result.stdout.readlines())
        string_out = str(process_result.stdout.decode('ascii'))
        
        output_list = string_out.splitlines()
        print(output_list)
        self.app.addListItems("mess",output_list)
        self.app.addListItem("mess","exitcode: "+str(process_result.returncode))
        
        solution = output_list[-1].strip("Best Solution is ").replace("  "," ")
        out0=""
        out1=""
        out2=""
        print(solution)


        for idx,cmd in enumerate(solution.split(" ")):
            if(idx <= self.level_params.nf0):
                out0+=cmd+" "

            else:
                if(idx <= self.level_params.nf0+self.level_params.nf1):
                    out1+=cmd+" "
                else:
                    out2+=cmd+" "


        self.app.setEntry("entry0",out0)
        self.app.setEntry("entry1",out1)
        self.app.setEntry("entry2",out2)
        
    def RED(self):
        self.app.thread(self.RED_CALL())
        self.check_entries() 

    def AnimateFrame(self):
        print("into animation")

        if self.animation.has_frames == False:
            print("finished")
            return

        
        fr = self.animation.currframe
        oldpos = self.animation.framelist[fr][0]+","+self.animation.framelist[fr][1]

        self.animation.currframe+=1
        if(self.animation.currframe == len(self.animation.framelist)-1):
            self.animation.has_frames=False

        fr = self.animation.currframe
        pos = self.animation.framelist[fr][0]+","+self.animation.framelist[fr][1]
        print("click from "+oldpos+" to "+pos)
    


        if pos != oldpos:
            self.app.hideImage("happyface"+oldpos)
            self.app.showImage("happyface"+pos)
            #print(self.level_params.starspositions)
        
            try:
                self.app.hideImage("happyheart"+pos)
                print("found a star")

            except:
                pass
            
            self.app.topLevel.update()
        
            #if(self.animation.framelist[fr][0] in self.level_params.starspositions[:][0]):
        #    if(self.animation.framelist[fr][1] in self.level_params.starspositions[:][1]):
        #        print("found a star")
        

        #time.sleep(0.5)
        #self.app.after(1000,self.AnimateFrame())
        self.app.after(0,self.AnimateFrame())
            
        return

    # Animates the submitted simulation
    def Animate(self):
        frames = []#.split("$$$$$$$$$$$$$$$$$")
        anim = Animation()
        roll = self.level_params.level_log
        for idx,item in enumerate(roll):
            if "$$$$$$$$$$$$$$$$$" in item:
                if "iteration" in roll[idx+1]: 
                    frames.append(roll[idx+3]) 


        oldx = str(self.level_params.startposition[0])
        oldy = str(self.level_params.startposition[2])
        oldangle = 0;

        #anim.framelist = frames
        for idx,frame in enumerate(frames):
            print("frame:"+frame)
            x = frame[22]#.strip("current position is (x")
            y = frame[25]#.strip("current position is (x"+x+",y")
            a = frame[34]
            anim.framelist.append(x+y+a)
            #self.AnimateFrame(x+","+y,oldx+","+oldy,1)
            
            oldx=x
            oldy=y

        anim.currframe=0
        anim.has_frames = True
        self.animation = anim
        self.has_animation = True
        self.AnimateFrame()
        
        print("DONE?")
    # TBD - refreshes the app before loading a level
    def Refresh(self):
        pass

    # the top-left menu, useless for now, just to add shit
    def menuPress(self,name):
        print("nothing happens, "+name)
        pass

if __name__ == '__main__':
    # Create an instance of your application
    App = RedConiglio()
    # Start your app !

    App.Start() 


 