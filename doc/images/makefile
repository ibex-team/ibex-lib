SVG=$(wildcard *.svg)
PNG=$(SVG:.svg=.png)

main: $(PNG)

%.png : %.svg
	inkscape -D -e $@ $^

clean:
	@rm -f $(PNG)
