# Suffix tabellen

## status

- De eigenlijke opgave is om deflate te implementeren. Dit is niet gedaan.

- De LCP wordt rechtstreeks opgebouwd zonder de nood van een LGP.

- De LGP wordt opgesteld op de naïve manier maar dit moet nog gecontrolleerd worden op fouten

- De LGP opstellen op de efficiente manier. Dit gebeurd niet. We slaan deze stap over
en slaan het direct op als een LCP. Dit is eigenlijk gewoon een LGP maar in andere volgorde. M.a.w. gaan we tijdens het construeren van de tabel, het direcht opslaan op de juiste LCP plaats. Hierdoor moeten we niet meer alles overlopen met de formule `LCP[i] = LGP[SA[i]]` aangezien we dit direct doen.

## Uitkomst prof

![oplossing prof](./oplossingProf.jpg)

## Log
```
index, Suffix Array, LCP
0	elling 1
1	ententententoonstelling 9
2	entententoonstelling 6
3	ententoonstelling 3
4	entoonstelling 4
5	entottententententoonstelling 0
6	g 0
7	hottentottententententoonstelling 0
8	ing 0
9	ling 1
10	lling 0
11	ng 1
12	nstelling 1
13	ntentententoonstelling 8
14	ntententoonstelling 5
15	ntentoonstelling 2
16	ntoonstelling 3
17	ntottententententoonstelling 0
18	onstelling 1
19	oonstelling 1
20	ottententententoonstelling 6
21	ottentottententententoonstelling 0
22	stelling 0
23	telling 2
24	tententententoonstelling 10
25	tentententoonstelling 7
26	tententoonstelling 4
27	tentoonstelling 5
28	tentottententententoonstelling 1
29	toonstelling 2
30	tottententententoonstelling 1
31	ttententententoonstelling 5
32	ttentottententententoonstelling 0
```
