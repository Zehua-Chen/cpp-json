all: bin/index.pdf

bin:
	mkdir bin
	
bin/index.pdf: index.tex bin
	pdflatex -output-directory=bin $<