#!/usr/bin/python

from gi.repository import Gtk

window = Gtk.Window()
window.connect_after('destroy', Gtk.main_quit)
window.add(Gtk.Image.new_from_file('/dev/stdin'))
window.show_all()
Gtk.main()
