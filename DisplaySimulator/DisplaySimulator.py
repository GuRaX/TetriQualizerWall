#!/usr/bin/python

import serial

import gtk
import gtk.gdk as gdk
from threading import Thread, Lock


COLS = 16
ROWS = 24
PIXELSIZE = 10
BORDER = 1


class SerialCommunication(Thread):
    def __init__(self, callback):
        Thread.__init__(self)
        self.ctrl = True
        self.callback = callback

    def run(self):
        port = serial.Serial("/dev/ttyUSB0")
        port.open()
        while(self.ctrl):
            line = port.read(18)
            pos = line.find(chr(170))
            line = "%s%s" % (line[pos:], line[:pos])
#            print line
            row = ord(line[1])
            data = line[2:]
#            print "%d:  %s" % (row/2, data)
            gdk.threads_leave()
            self.callback(row/2, data)
            gdk.threads_enter()
        port.close()

    def close(self):
        self.ctrl = False


class PyApp(gtk.Window):

    def __init__(self):
        super(PyApp, self).__init__()

        self.color = (0.0, 0.07, 0.14, 0.21, 0.28, 0.35, 0.42, 0.47, 0.54, 0.63, 0.70, 0.77, 0.83, 0.89, 0.95, 1.0)
        self.data = ((0,0,0,0,0,0,15,0,255,0,0,0,0,0,0,0),
                     (0,0,0,0,0,0,240,0,255,0,0,0,0,0,0,0),
                     (0,0,0,0,0,0,15,0,255,0,0,0,0,0,0,0),
                     (0,0,0,0,0,0,240,0,255,0,0,0,0,0,0,0),
                     (0,0,0,0,0,0,15,0,255,0,0,0,0,0,0,0),
                     (0,0,0,0,0,0,240,0,255,0,0,0,0,0,0,0),
                     (0,0,0,0,0,0,15,0,255,0,0,0,0,0,0,0),
                     (0,0,0,0,0,0,240,0,255,0,0,0,0,0,0,0),
                     (0,0,0,0,0,0,15,0,255,0,0,0,0,0,0,0),
                     (0,0,0,0,0,0,240,0,255,0,0,0,0,0,0,0),
                     (0,0,0,0,0,0,15,0,0,0,0,0,0,0,0,0),
                     (0,0,0,0,0,0,240,0,0,0,0,0,0,0,0,0),
                     (0,0,0,0,0,0,15,0,0,0,0,0,0,0,0,0),
                     (0,0,0,0,0,0,240,0,0,0,0,0,0,0,0,0),
                     (0,0,0,0,0,0,15,0,0,0,0,0,0,0,0,0),
                     (0,0,0,0,0,0,240,0,0,0,0,0,0,0,0,0),
                     (0,0,0,0,0,0,15,0,0,0,0,0,0,0,0,0),
                     (0,0,0,0,0,0,240,0,0,0,0,0,0,0,0,0),
                     (0,0,0,0,0,0,15,0,0,0,0,0,0,0,0,0),
                     (0,0,0,0,0,0,240,0,0,0,0,0,0,0,0,0),
                     (0,0,0,0,0,0,15,0,0,0,0,0,0,0,0,0),
                     (0,0,0,0,0,0,240,0,0,0,0,0,0,0,0,0),
                     (0,0,0,0,0,0,15,0,0,0,0,0,0,0,0,0),
                     (0,0,0,0,0,0,240,0,0,0,0,0,0,0,0,0) )

        self.resize(COLS*PIXELSIZE, ROWS*PIXELSIZE)
        self.connect("destroy", self.myApp_Quit)

        area = gtk.DrawingArea()
        area.connect("expose-event", self.expose)
        self.add(area)
        self.show_all()
        self.worker = SerialCommunication(self.threadCall)
        self.worker.start()


    def threadCall(self, rowId, data):
        print "Id: %d  Data: %s" % (rowId, data)


    def myApp_Quit(self, obj):
        self.worker.close()
        gtk.main_quit()


    def expose(self, widget, event):
        cr = widget.window.cairo_create()
        for c in range(COLS):
            for r in range(ROWS):
                red = self.data[r][c] & 0x0F
                blue = (self.data[r][c] >> 4) & 0x0F
                cr.set_source_rgb(self.color[red],0,self.color[blue])
                cr.rectangle( c*PIXELSIZE + BORDER, r*PIXELSIZE + BORDER, PIXELSIZE-2*BORDER, PIXELSIZE-2*BORDER)
                cr.fill()


PyApp()
gtk.main()
