cc = arm-linux-gcc
prom = app
deps = $(shell find ./ -maxdepth 1 -name '*.h'|sed 's%.*/%%')
src = $(shell find ./ -maxdepth 1 -name '*.c'|sed 's%.*/%%')
obj = $(src:%.c=%.o)
lib = -lm -ljpeg

$(prom):$(obj)
	$(cc) -o $(prom) $(obj) $(lib)

%.o:%.c $(deps)
	$(cc) -c $< -o $@ $(lib)

clean:
	rm -rf $(obj) $(prom)
