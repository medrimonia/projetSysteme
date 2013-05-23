OBJECTS=presentation.pdf rapport_16mai.pdf rapport_int.pdf

all: ${OBJECTS}

%.pdf: %.tex
	pdflatex $< && pdflatex $<


clean:
	rm *.log
	rm *.aux
	rm *.pdf