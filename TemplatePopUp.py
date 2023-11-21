from tkinter import *
from tkinter import ttk
from platform import system as psys
import os
from Schemes.colors import *
from Schemes.constants import *
from PIL import Image, ImageTk

#System based imports
if psys() == "Windows":
    from ctypes import windll
has_style   =   False

class PopUpTemplate:
    def __init__(self):
        self.runningOs = psys()
        self.ventana=Tk()
        self.ventana.overrideredirect(True)
        self.ventana.minsize(POPWIDTH, POPHEIGHT)
        self.ventana.resizable(width=NO, height=NO)

        # self.ventana = Canvas(self.venta, background=BACKGROUND, width=WIDTH, height=HEIGHT, highlightthickness=0)
        self.mainPanel = Canvas(self.ventana, background=BACKGROUND, width=POPWIDTH, height=POPHEIGHT-TOPHEIGHT, highlightthickness=0)
        self.mainPanel.place(x=0,y=TOPHEIGHT)

        topbar = Canvas(self.ventana, background=TOPBAR, width=POPWIDTH, height=TOPHEIGHT, highlightthickness=0)
        topbar.place(x=0,y=0) 
        topbar.bind('<B1-Motion>', self.move_window)

        topbar.bind('<Button-1>', self.get_pos)

        self.exitImage = self.cargar_img("quitImage.png")
        self.exitButton = Button(self.ventana, image=self.exitImage, width=TOPHEIGHT, height=TOPHEIGHT-2, border=0, bg=TOPBAR)
        self.exitButton.place(x=POPWIDTH,y=0, anchor=NE)
        self.exitButton.bind("<Enter>", self.on_enterExit)
        self.exitButton.bind("<Leave>", self.on_leaveExit)
        self.exitButton.bind('<Button-1>', self.close)

        if self.runningOs == "Windows":
            self.set_appwindow()

    def on_enterExit(self, e):
        self.exitButton['background'] = REDBG

    def on_leaveExit(self, e):
        self.exitButton['background'] = TOPBAR

    def close(self, e):    
        self.ventana.destroy()

    def set_appwindow(self):
        global has_style
        GWL_EXSTYLE=-20
        WS_EX_APPWINDOW=0x00040000
        WS_EX_TOOLWINDOW=0x00000080
        if not has_style:
            hwnd = windll.user32.GetParent(self.ventana.winfo_id())
            style = windll.user32.GetWindowLongW(hwnd, GWL_EXSTYLE)
            style = style & ~WS_EX_TOOLWINDOW
            style = style | WS_EX_APPWINDOW
            res = windll.user32.SetWindowLongW(hwnd, GWL_EXSTYLE, style)
            self.ventana.withdraw()
            self.ventana.after(10, lambda:self.ventana.wm_deiconify())
            has_style=True


    def cargar_img(self, nombre, width=0, height=0):
        ruta = os.path.abspath(os.path.join("Views\Assets", nombre)) #crea acceso a la ruta assets dentro de la carpeta del programa
        if not os.path.exists(ruta):
            ruta = os.path.abspath(os.path.join("Assets", nombre))
        if self.runningOs == "Linux":
            ruta = os.path.join(os.getenv("ASSETS_PATH_LINUX"), nombre)
        img = Image.open(ruta)
        if width != 0 and height != 0:
            img = img.resize((width, height))
        img = ImageTk.PhotoImage(img)
        return img #retorna la imagen 
    
    def build(self):
        self.placeWidgets()
        self.bindWidgets()
        self.ventana.mainloop()
    
    def get_pos(self, event):
        xwin = self.ventana.winfo_x()
        ywin = self.ventana.winfo_y()
        startx = event.x_root
        starty = event.y_root

        self.ywin = ywin - starty
        self.xwin = xwin - startx

    def move_window(self, event):
        self.ventana.geometry((str(POPWIDTH)+"x"+str(POPHEIGHT)) + '+{0}+{1}'.format(event.x_root + self.xwin, event.y_root + self.ywin))
    
    def clearFrame(self, window):
        for widget in window.winfo_children():
            widget.destroy()

if __name__ == "__main__":
    viewTemplate = PopUpTemplate()
    viewTemplate.build()