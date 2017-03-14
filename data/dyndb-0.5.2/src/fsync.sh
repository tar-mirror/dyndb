#! /bin/sh
# try to find fsync
#
cat >conftest.c <<EOF
int main() {
	return fsync(1);
}
EOF
./compile.sh -c conftest.c  2>/dev/null >/dev/null
./link.sh conftest conftest.o $i 2>/dev/null >/dev/null
if test $? -eq 0 ; then
	rm -f conftest.c conftest.o conftest
	exit 0;
fi
rm -f conftest.c conftest.o conftest
./compile.sh -c fsync.c && echo "fsync.o"
