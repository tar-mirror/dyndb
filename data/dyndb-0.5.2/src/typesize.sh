#! /bin/sh
cat >conftest.c <<EOF
int main()
{
	TYPE t;
	return sizeof(t);
}
EOF
for i in "short" "int" "long " "unsigned short" "unsigned int" "unsigned long" \
	"long long" "unsigned long long" ; do 
	rm -f conftest.o
	./compile.sh -DTYPE="$i" -c conftest.c >/dev/null 2>/dev/null
	if test -f conftest.o ; then
	  if ./link.sh conftest conftest.o ; then
	  	./conftest
		x=$?
		if test "$x" -ne 0 ; then
			p=`echo $i | sed 's/ /_/g' | tr "[a-z]]" "[A-Z]"`
			echo "#define SIZEOF_$p $x"
		fi
	  fi
	fi
done
rm -f conftest.c conftest.o conftest
exit 0
