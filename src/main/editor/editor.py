 
# import the library
from appJar import gui

#table color - number
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

# Example of a semi-structured application
class EditorGui():

    # Build the GUI
    def Prepare(self, app):
        # Form GUI
        app.setTitle("RedConiglio Generate Level")
        app.setFont(16)
        app.setStopFunction(self.BeforeExit)
        self.title = "RedConiglio Generate Level"

        # game grid
        for it in range(10):
            for jt in range(10):

                app.addButton(str(it)+","+str(jt), self.Dummy , it,jt) 
                app.setButtonBg(str(it)+","+str(jt), "grey")

        app.addHorizontalSeparator(10,0,10, colour="red")
        app.addButton("< ",self.Command,11,0)
        app.setButtonBg("< ","white")
        app.addButton("^ ",self.Command,11,1)
        app.setButtonBg("^ ","white")
        app.addButton("> ",self.Command,11,2)
        app.setButtonBg("> ","white")
        
        app.addButton("f0 ",self.Command,12,0)
        app.setButtonBg("f0 ","white")
        app.addButton("f1 ",self.Command,12,1)
        app.setButtonBg("f1 ","grey")
        app.addButton("f2 ",self.Command,12,2)
        app.setButtonBg("f2 ","grey")

        app.addButton("r ",self.Command,13,0)
        app.setButtonBg("r ","grey")
        app.addButton("g ",self.Command,13,1)
        app.setButtonBg("g ","grey")
        app.addButton("b ",self.Command,13,2)
        app.setButtonBg("b ","grey")

        app.addButton("Pr ",self.Command,14,0)
        app.setButtonBg("Pr ","grey")
        app.addButton("Pg ",self.Command,14,1)
        app.setButtonBg("Pg ","grey")
        app.addButton("Pb ",self.Command,14,2)
        app.setButtonBg("Pb ","grey")

        app.addLabel( "lf0 ","#f0",11,5)
        app.addLabel( "lf1 ","#f1",12,5)
        app.addLabel( "lf2 ","#f2",13,5)
        app.addNumericEntry( "nf0 ",11,6,colspan=6)
        app.addNumericEntry( "nf1 ",12,6,colspan=6)
        app.addNumericEntry( "nf2 ",13,6,colspan=6)

        app.addLabel( "sss","addstars",15,4,colspan=2)
        app.addEntry( "sss1",15,6,colspan=6)
        app.addLabel( "zzz","setstart",16,4,colspan=2)
        app.addEntry( "zzz1",16,6,colspan=6)
        app.addLabel( "lll","levelname",17,4,colspan=2)
        app.addEntry( "lll1",17,6,colspan=6)       
        app.addButton("Sv",self.Save,17,0)
        #app.addButton("Ld",self.Load,17,1) 
        self.app = app
        return app
        
    # Build and Start your application
    def CallMe(self,parent,title):
        self.title = title
        self.parent = parent

        #app = qualcosa
        #self.app = app
        #app.go()

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
        return 1#self.app.yesNoBox("Confirm Exit", "Are you sure you want to exit the application?")
    
    # Define method that is executed when the user clicks on the submit buttons
    # of the form

    def Dummy(self,title):
        curr_color = self.app.getButtonBg(title)
        if(curr_color == "grey"):
            print("daje")
    
        print(curr_color)

        new_color = (COLOR_DIC[curr_color] +1) % 4
        self.app.setButtonBg(title, COLOR_DIC[new_color] )

    def Command(self,title):
        print("pressed "+title)

        if self.app.getButtonBg(title) == "white":    
            self.app.setButtonBg(title,"grey")
        
        else:
            if self.app.getButtonBg(title) == "grey":  
                self.app.setButtonBg(title,"white")

            else:
                self.app.setButtonBg(title,"grey")
                print("unset, setting [TBD]")

    def Save(self):

        filename = self.app.getEntry("lll1")+".redlevel"
        print("levelname is "+filename)
        thisfilepath="../../test/levels/"
        outfile = open(thisfilepath+filename,"w")
        #outfile = open(filename,"w")
        outfile.write("# REDCONIGLIO LEVEL DESCRIPTION FILE\n")
        outfile.write(r"%name%="+self.app.getEntry("lll1")+"\n")
        outfile.write(r"%map%="+"\n")

        starstring = self.app.getEntry("sss1")
        startstring = self.app.getEntry("zzz1")


        for it in range(10):    
            for jt in range(10):

                offset = 0
                offset += 100*(str(it)+","+str(jt) in starstring)
                
                #in start case!
                if str(it)+","+str(jt) in startstring:
                    if "R" in startstring:
                        offset +=10

                    if "U" in startstring:
                        offset +=20

                    if "L" in startstring:
                        offset +=30

                    if "D" in startstring:
                        offset +=40

                outfile.write(str(offset+COLOR_DIC[self.app.getButtonBg(str(it)+","+str(jt))]))
                outfile.write(" ")



            outfile.write("\n") 


        outfile.write(str(int(self.app.getEntry("nf0 ")))+" ")    
        outfile.write(str(int(self.app.getEntry("nf1 ")))+" ")
        outfile.write(str(int(self.app.getEntry("nf2 ")))+"\n")

        counter = 0
        for symbol in ["< ","^ ","> ","f0 ","f1 ","f2 ","Pr ","Pg ","Pb "]:
            if (self.app.getButtonBg(symbol)!="grey"):
                counter += 1
        outfile.write(str(int(counter))+" ")        
        
        counter = 1
        for symbol in ["r ","g ","b "]:
            if (self.app.getButtonBg(symbol)!="grey"):
                counter += 1
        outfile.write(str(int(counter))+"\n")
        counter = 0

        for symbol in ["< ","^ ","> ","f0 ","f1 ","f2 ","Pr ","Pg ","Pb "]:
            outfile.write( (str(int(counter))+" ")*(self.app.getButtonBg(symbol)!="grey"))
            counter += 1
        outfile.write("\n")
        
        #this is always present
        counter = 1
        outfile.write("0 ")
        for symbol in ["r ","g ","b "]:
            outfile.write( (str(int(counter))+" ")*(self.app.getButtonBg(symbol)!="grey"))
            counter += 1

        outfile.write("\n")    
        # Just some metadata
        outfile.write(r"$stars_positions$="+starstring+"\n")
        outfile.write(r"$start_position$="+self.app.getEntry("zzz1")+"\n")
        outfile.write(r"$valid_symbols$=")
        outfile.write(r"")

        for symbol in ["< ","^ ","> ","f0 ","f1 ","f2 ","r ","g ","b ","Pr ","Pg ","Pb "]:
            outfile.write( (symbol[:-1]+",")*(self.app.getButtonBg(symbol)!="grey"))

        outfile.write("\n")    
        outfile.write(r"$f0slot$="+str(self.app.getEntry("nf0 "))+"\n")
        outfile.write(r"$f1slot$="+str(self.app.getEntry("nf1 "))+"\n")
        outfile.write(r"$f2slot$="+str(self.app.getEntry("nf2 "))+"\n")#.replace("None","0")
        outfile.close()

    def Load(self):

        filename = self.app.getEntry("lll1")+".redlevel"
        print("tryin to load "+filename)
        print("nothing happens...")


if __name__ == '__main__':
    # Create an instance of your application
    App = EditorGui()
    # Start your app !
    App.Start() 
