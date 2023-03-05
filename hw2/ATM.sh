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
function replace_in_accounts(){
	total_line=`cat account.txt | wc -l`
	line_1=`echo "$1-1" | bc`
        t=`echo "$total_line-$1" | bc`
	tails=`tail -n $t account.txt`
	echo "tails" $tails
	heads=`head -n $line_1 account.txt`
	rm account.txt
	cnt=1
        line=`echo `
        for i in $heads
        do
                r=$((cnt % 3))
                if [[ $r -eq 1 ]]
                then
                        echo here
                        line=`echo $i`
                elif [[ $r -eq '2' ]]
                then
                        echo there
                        line=`echo $line $i`
                else
                        line=`echo $line $i`
                        echo $line >> account.txt

                fi



                cnt=$((cnt + 1))
	done
	cat account.txt
        echo $2 $3 $4 >> account.txt
	cat account.txt
	cnt=1
	line=`echo `
	for i in $tails
	do
		r=$((cnt % 3))
		if [[ $r -eq 1 ]]
		then
			echo here
			line=`echo $i`
		elif [[ $r -eq '2' ]]
		then
			echo there
			line=`echo $line $i`
		else
			line=`echo $line $i`
			echo $line >> account.txt

		fi



		cnt=$((cnt + 1))
	
			
	done
	echo "********************************************************************"
}

function find_line_file(){
	find_line=`cat account.txt | cut -d " " -f 1 | grep -n $1| cut -d ":" -f 1`
	echo $find_line
	return $find_line
}


function find_acc_balance(){
	acc_balance=`echo $3`
	return $acc_balance
}

function transaction(){
	read -p "enter your account number : "  origin
	read -p "enter destination account number : " destination 
	read -p "enter the amount : " amount 

	find_line_file $origin 
	find_line_origin=`echo $?`
	origin_info=`head -n $find_line_origin account.txt | tail -n 1`
	find_acc_balance $origin_info
	acc_balance_origin=`echo $?` 
	echo $find_line_origin $origin_info $acc_balance_origin
	if [[ $amount -gt $acc_balance_origin ]]
	then
		echo "your money doesnt enough!"
        else
        	now_date=`date +%Y-%m-%d_%H-%M-%S`
		echo $origin $destination $now_date $amount >> transactions.txt

		remaining=`echo "$acc_balance_origin-$amount" | bc`
		echo $amount $remaining
		final_origin_info=`echo $origin_info | sed -e 's/'$acc_balance_origin'/'$remaining'/g'`
		echo $final_origin_info
		replace_in_accounts $find_line_origin $final_origin_info
		cat account.txt
		echo "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"		
		find_line_file $destination
	        find_line_destination=`echo $?`
        	destination_info=`head -n $find_line_destination account.txt | tail -n 1`
        	find_acc_balance $destination_info
        	acc_balance_destination=`echo $?` 
		addition=`echo "$acc_balance_destination+$amount" | bc`
		final_destination_info=`echo $destination_info | sed -e 's/'$acc_balance_destination'/'$addition'/g'`
		echo $find_line_destination $final_destination_info
		replace_in_accounts $find_line_destination $final_destination_info
		cat account.txt



        fi
	
}
function list_of_transactions(){
	read -p "enter your account number :  " acc_num
	lines=`cat transactions.txt | cut -d " " -f 1 | grep -n $acc_num| cut -d ":" -f 1`
	
	for i in $lines
	do
		echo `sed -n $i'p' transactions.txt` 

	done
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
  
