FILES=`ls | grep "fail*"`

make -C ../../build/

for value in $FILES
do
	RES=`./../../build/webserv ./$value`;
	if [[ $? -eq 0 ]]; then
		if [[ -z $RES ]]; then
			# printf "$RES";
			printf "$value: \e[31mOK\e[0m\n";
		else
			# printf "$RES";
			printf "$value: \e[32mKO\e[0m\n";
		fi
	else 
		printf "$value: \e[32mFAIL\e[0m\n";
	fi
done
