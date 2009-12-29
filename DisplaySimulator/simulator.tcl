#!/usr/bin/wish

# display configuration
set pixelW 16
set pixelH 25
set border 1
set pixelSize 15
set curr_data [list 0  0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0]
set status 0
set tty 0
set tmp ""

###############################################################################
#   Functions
###############################################################################

proc toASCII { _char } {
    scan $_char %c _value
    return $_value
}



proc read_serial { } {
    global tty
    global status
    global curr_data
    global pixelW
    global tmp

    set val [read $tty]
    set valINT [toASCII $val]


    puts $valINT
    
#[toASCII $val]  
    
#    if { $status > 0 } {
#        lset curr_data [expr $status -1] "$valINT"
#        incr status
#        if { $status < 17 } {
#        #[expr $pixelW +1] } {
#            puts "Done!!  $curr_data"
#            set status 0
#        }
#    }

#    if { $valINT == 170 && $status == 0 } {
#        puts "Start Condition  $valINT = 170  $status = 0"
#        incr status
#    }

#    puts "ROW:   LED:   Val: "
}

###############################################################################
#   GUI Functions/Procedures
###############################################################################

proc btn_start {} {
    global tty
    set tty [open /dev/ttyUSB0 r]
    puts "Open Port"
    fconfigure $tty -mode 9600,n,8,1 -blocking 0
    fileevent $tty readable {read_serial}
}

proc btn_stop {} {
    global tty
    if { $tty != 0 } {
        puts "Closed Port"
        close $tty
    }
    exit
}


###############################################################################
#   Main Function
###############################################################################

canvas .c -width [expr $pixelW * $pixelSize] -height [expr $pixelH * $pixelSize]
button .start -text "Start" -command btn_start
button .stop  -text "Exit"  -command btn_stop
pack .c .start .stop

for {set col 0 } { $col < $pixelH } { incr col } {
    for {set row 0 } { $row < $pixelW } { incr row } {
        .c create rect [expr $row * $pixelSize + $border] \
                       [expr $col * $pixelSize + $border] \
                       [expr ($row +1) * $pixelSize - $border] \
                       [expr ($col +1) * $pixelSize - $border] \
                       -fill white
    }
}


