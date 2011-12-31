 #!/bin/bash
# autoftp.sh
lokal_file=Release/main.out
remote_file=main.out

ftp -n <<EOFTP
	open 192.168.1.1
	user anonymous
	put $lokal_file $remote_file
	quit
EOFTP
