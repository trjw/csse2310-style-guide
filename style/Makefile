DEST=/local/courses/csse2310/bin
RESOURCES=/local/courses/csse2310/resources
SCRIPTS=2310reformat.sh 2310stylecheck.sh
CONFIG_FILES=.clang-tidy .clang-format
install:
	install --mode=0775 --group=csse2310 $(SCRIPTS) $(DEST)
	install --mode=0664 --group=csse2310 $(CONFIG_FILES) $(RESOURCES)
