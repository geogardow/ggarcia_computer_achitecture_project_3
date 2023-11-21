from TemplatePopUp import PopUpTemplate
import tkinter as tk
from Schemes.colors import *
from Schemes.constants import *
import tkinter.font as font
import socket


class Interface(PopUpTemplate):
    def __init__(self):
        super().__init__()

        MAINFONT = font.Font(family='Montserrat Light', size=14)
        SECONDARYFONT = font.Font(family='Montserrat Light', size=26)

        self.udp_ip = "192.168.44.15"  # Use 0.0.0.0 to bind to all available interfaces
        self.udp_port = 8888

        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.sock.bind((self.udp_ip, self.udp_port))

        print("Listening on {}:{}".format(self.udp_ip, self.udp_port))

        self.buttonImage = self.cargar_img("button.png", BTNDIMENSION-10, BTNDIMENSION-10)

        self.leftButton = tk.Button(self.mainPanel, image=self.buttonImage, bg=BACKGROUND,\
                                   compound="center", text="Left", border=0, font=MAINFONT,\
                                      fg=FAKEWHITE, activebackground=BACKGROUND)
        
        self.rightButton = tk.Button(self.mainPanel, image=self.buttonImage, bg=BACKGROUND,\
                                   compound="center", text="Right", border=0, font=MAINFONT,\
                                      fg=FAKEWHITE, activebackground=BACKGROUND)
        
        self.upButton = tk.Button(self.mainPanel, image=self.buttonImage, bg=BACKGROUND,\
                                   compound="center", text="Up", border=0, font=MAINFONT,\
                                      fg=FAKEWHITE, activebackground=BACKGROUND)
        
        self.downButton = tk.Button(self.mainPanel, image=self.buttonImage, bg=BACKGROUND,\
                                   compound="center", text="Down", border=0, font=MAINFONT,\
                                      fg=FAKEWHITE, activebackground=BACKGROUND)
        
        self.autoButton = tk.Button(self.mainPanel, image=self.buttonImage, bg=BACKGROUND,\
                                   compound="center", text="Auto", border=0, font=MAINFONT,\
                                      fg=FAKEWHITE, activebackground=BACKGROUND)
        
        self.manualButton = tk.Button(self.mainPanel, image=self.buttonImage, bg=BACKGROUND,\
                                   compound="center", text="Manual", border=0, font=MAINFONT,\
                                      fg=FAKEWHITE, activebackground=BACKGROUND)
        
    def changeButtonsState(self, value):
        self.leftButton.config(state=value)
        self.rightButton.config(state=value)
        self.upButton.config(state=value)
        self.downButton.config(state=value)

    def placeWidgets(self):
        self.leftButton.place(x=POPWIDTH//3, y=POPHEIGHT//2, anchor="center")
        self.rightButton.place(x=2*POPWIDTH//3, y=POPHEIGHT//2, anchor="center")
        self.upButton.place(x=POPWIDTH//2, y=POPHEIGHT//4, anchor="center")
        self.downButton.place(x=POPWIDTH//2, y=3*POPHEIGHT//4, anchor="center")
        self.autoButton.place(x=POPWIDTH-20, y=20, anchor=tk.NE)
        self.manualButton.place(x=POPWIDTH-20, y=POPHEIGHT-50, anchor=tk.SE)

    def pressButton(self, event):
        self.data, self.addr = self.sock.recvfrom(1024)  # Buffer size is 1024 bytes
        print("Received message from ESP32: '{}' from {}".format(self.data.decode("utf-8"), self.addr))
        buttonText = event.widget.cget("text")
        if buttonText == "Manual":
            self.changeButtonsState(tk.NORMAL)
            message = "1"
        elif buttonText == "Auto":
            self.changeButtonsState(tk.DISABLED)
            message = "0"
        elif buttonText == "Up":
            message = "2"
        elif buttonText == "Down":
            message = "3"
        elif buttonText == "Left":
            message = "4"
        elif buttonText == "Right":
            message = "5" 
        self.sock.sendto(message.encode("utf-8"), self.addr)

    def releaseButton(self, event):
        self.data, self.addr = self.sock.recvfrom(1024)  # Buffer size is 1024 bytes
        print("Received message from ESP32: '{}' from {}".format(self.data.decode("utf-8"), self.addr))
        message = "9"
        self.sock.sendto(message.encode("utf-8"), self.addr)
    
    def bindWidgets(self):
        self.leftButton.bind('<ButtonPress-1>', self.pressButton)
        self.rightButton.bind('<ButtonPress-1>', self.pressButton)
        self.upButton.bind('<ButtonPress-1>', self.pressButton)
        self.downButton.bind('<ButtonPress-1>', self.pressButton)
        self.autoButton.bind('<ButtonPress-1>', self.pressButton)
        self.manualButton.bind('<ButtonPress-1>', self.pressButton)
        self.leftButton.bind('<ButtonRelease-1>', self.releaseButton)
        self.rightButton.bind('<ButtonRelease-1>', self.releaseButton)
        self.upButton.bind('<ButtonRelease-1>', self.releaseButton)
        self.downButton.bind('<ButtonRelease-1>', self.releaseButton)
        self.autoButton.bind('<ButtonRelease-1>', self.releaseButton)
        self.manualButton.bind('<ButtonRelease-1>', self.releaseButton)

        
    def setSuccessCallback(self, callback):
        self.successCallback = callback


if __name__ == "__main__":
    employeeLogIn = Interface()
    employeeLogIn.build()