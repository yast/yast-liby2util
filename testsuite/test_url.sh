Y2SLOG_FILE=/dev/stderr
Y2SLOG_DEBUG=1

for i in `cat urltests`;
do
	./test_url.prg "$i"
done
