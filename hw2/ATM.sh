function print_menu (){
        echo "welcome to my ATM :)" 
        echo
        echo "1. make a new account"
        echo "2. transact from an account to another"
        echo "3. get the list of transactions of an account"
        echo "4. get the list of accounts of a user "
        echo "5. exit"
	echo
        echo "---------------------------------------------------------------"
        echo
        echo "Enter the option number of your choice to continue : "
}
function new_account(){
	read -p "enter your account number , your name , your account balance : "  input
	echo $input >> account.txt
}
function transaction(){
	echo "transaction"
}
function list_of_transactions(){
	echo "list of transactions"
}
function list_of_accounts(){
	read -p "enter your name :  " name
	cat account.txt | grep $name
}



while :
do
	print_menu 
	read option

	case $option in 
        	"1")
			new_account
			;;

 	       "2")
			transaction
			;;
	        "3")
			list_of_transactions
			;;
        	"4")
			list_of_accounts
			;;
		"5")
			break
			;; 
	esac
	echo 
	echo 
	echo 
	echo 
done	

