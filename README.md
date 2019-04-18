
Utilities to manipulate character separated text fields.
eg
/etc/passwd

collect - summarizes a specified field
eg vacations.txt
	peter:paris:2007
	paul:london:2010
	allen:berlin:2010
	alice:rome:2005
	bob:london:2010
	peter:rome:2007
	alice:rome:2009

sort vacations.txt | <b>collect -c1 -d: </b>

	alice:rome:2005
	alice:rome:2009
	allen:berlin:2010
	bob,paul:london:2010
	peter:paris:2007
	peter:rome:2007

sort vacations.txt | <b>collect -c2 -d: </b>

	alice:rome:2005
	alice:rome:2009
	allen:berlin:2010
	bob:london:2010
	paul:london:2010
	peter:paris,rome:2007

sort vacations.txt | <b>collect -c3 -d: </b>

	alice:rome:2005,2009
	allen:berlin:2010
	bob:london:2010
	paul:london:2010
	peter:paris:2007
	peter:rome:2007

Rerorganise the fields with project

sort vacations.txt | <b>collect -c1 -d:</b> | <b>project -d: -f2,3,1</b> | sort

	berlin:2010:allen
	london:2010:bob,paul
	paris:2007:peter
	rome:2005:alice
	rome:2007:peter
	rome:2009:alice

Summarize the last field where number of fields varies

project -f '$,2-$_1' | sort | collect -c1 | project -f '2-$,1'
