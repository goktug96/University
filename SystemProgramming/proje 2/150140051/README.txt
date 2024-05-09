messagebox	->	modified version of scull
-----------------------------------------------------------------------

./setup	->	use to setup messagebox
				will run make command to compile module
				remove messagebox if exists
				ins messagebox module
				remove messagebox nod if exists
				make messagebox nod with dynamic major number selection
				give write permission to users for messagebox

./delete_test <username>	->	will delete all messages sent to <username>

./read_mode_test <1 or 0>	->	1 to incude read, 0 to exclude read

./unread_limit_test <limit>	->	set maximum number of unread messages that a user can have

echo "@<to> <message>" >> /dev/messagebox	->	send message <message> to <to>

cat /dev/messagebox	->	read messages sent to executor of this command
