.PHONY: all clean

all: style_guide.tex
	pdflatex style_guide.tex
	pdflatex style_guide.tex

clean:
	rm style_guide.pdf *.aux *.log *.out

# The following applies to the clang-format and clang-tidy configuration files
# on moss
install:
	install --mode=0664 --group=csse2310 .clang-format .clang-tidy /local/courses/csse2310/resources
