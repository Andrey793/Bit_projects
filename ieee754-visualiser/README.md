Консольная утилита, которая принимает число с плавающей точкой и выводит его прдеставление в форме +-1 * 2^exp * M и битовое представление. Это проделаывается для float и double представлений введенного числа. Работает для систем, где float и double имеют размер 32/64. 

### Пример:
> ./vis -128
> Float representation: 
	-1 * 2^(7) * 1.000000000 
	Sign:1  Exp:10000110  Frac:00000000000000000000000
Double representation: 
	-1 * 2^(7) * 1.0000000000000000 
	Sign:1  Exp:10000000110  Frac:0000000000000000000000000000000000000000000000000000

> ./vis 0.3 
> Float representation: 
	1 * 2^(-2) * 1.200000048 
	Sign:0  Exp:01111101  Frac:00110011001100110011010
Double representation: 
	1 * 2^(-2) * 1.2000000000000000
	Sign:0  Exp:01111111101  Frac:0011001100110011001100110011001100110011001100110011

> ./vis 1e100
> float representation:: Numerical result out of range
double representation:: Numerical result out of range
This number cant be represented as a float or double, so it cant be visualized

