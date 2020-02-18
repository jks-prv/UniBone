# Test of M9312 bootroms
# in emulated CPU20, with emulated DL11


# CONOLE EMULATOR = 23-248F1.lst - 11/34 console emualtor
# BOOTROM1=23-751A9.lst="DL" RL11
# BOOTROM2="DK" RK11/"DT" DecTape
# BOOTROM3=23-767A9.lst "DU" MSCP
# BOOTROM4=  empty

             |

dc			# "device without cpu" menu

m i			# gimme memory

sd m9312		# further commands form M9312
p v 5			# verbosity "debug"

p bl DIAG		# set start label. testing "before ROM load"

p cer 23-248F1.lst	# plug console emulator ROM into socket

p br1 23-751A9.lst	# plug RL BOOT ROM into socket1


p			# show all

e 765000	# verify: timeout
e 765776	# verify: timeout
e 773000
e 773176
en m9312		# online

e 765000	# verify: data
e 767776
e 773000
e 773176

.print Test of bootvector redirection timeout
e 24		# not redirected: 0
pwr		# ACLO -> enable 24 -> 773024 for 300 ms
e 24		# redirected 773024 =>173000 ?
.wait 500
e 24		# 0 again?

.print Test of bootvector redirection DATI count

e 24		# not redirected: 0
pwr
e 24		# redirected 773024 => 173000 ?
e 26		# redirected 773026 => 000340 ?
e 24		# 2 DATIs count: no redirection, => 0 ?

.print Now CONSOLE EMULATOR = 23-248F1.lst visible at 765000-765776
.print BOOTROM1 visible at 773000-773176
.print BOOTROM2 visible at 773200-773376
.print BOOTROM3 visible at 773400-773576
.print BOOTROM4 visible at 773600-773776
