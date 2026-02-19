DEST=/local/courses/csse2310/bin
RESOURCES=/local/courses/csse2310/resources
SCRIPTS=2310reformat.sh \
	2310stylecheck.sh \
	2310checkcomments.sh \
	2310checkfunctioninternalcomments.sh \
	2310getcomments.sh \
	2310getdefns.sh \
	2310getfilecomment.sh
CONFIG_FILES=.clang-tidy .clang-format .csse2310.doxyconfig
install:
	install --mode=0775 --group=csse2310 $(SCRIPTS) $(DEST)
	install --mode=0664 --group=csse2310 $(CONFIG_FILES) $(RESOURCES)
