# Performance

- Latency

- Throughput

- CPI: *cycles per instruction*

# MIPS

- ## R Format

    op|rs|rt|rd|shamt|funct
    :--:|:--:|:--:|:--:|:--:|:--:
    000000|-----|-----|-----|-----|------
    immer 0|source|target|dest|shift amount|Arith. Mode

    z.B. SUB, AND

- ## I Format

    op|rs|rt|immediate
    :--:|:--:|:--:|:--:
    ------|-----|-----|----------------
    Opcode|source|target|16bit Immediate

    z.B. ADDI, BEQ

- ## J Format

    op|addr
    :--:|:--:
    ------|--------------------------------
    Opcode|Shifted left by 2bits (wordsize=4) 
    '|and stays in the same 4bit region

    z.B. J, JAL, JR

## Pseudoinstruktionen

```MIPS
move $t0, $t1       ->  add $t0, $t1, $zero

blt $t1, t2, label  ->  slt $at, $t1, $t2
                        bne $at, $zero, $label

bne $t1, $t2, far_l ->  beq $t1, $t2, tmp_l
                        j far_l
                        tmp_l: ...

li $t1, imm         ->  ori $t1, $zero, imm

load_long $t1, imm  ->  lui $t1, imm_high
                        ori $t1, $t1, imm_low
```

## Stack

![stack](https://www.bwuah.me/RO/c1.PNG)

## Overflow

- exception

- pre defined position in code

- U versions of OPs come without overflow check

# Steuerwerk

## ALU

```
0000 = and
0001 = or
0010 = add
0110 = subtract
0111 = slt
1100 = nor
```

???

# Pipelines

- k Stufen Pipeline

## Strukturkonflikte

- selbe Ressource benötigt

- Harvard Arch schafft Abhilfe

## Datenabhängigkeit

```mips
add       $t1, $t2, $t3
#          v    
addi $t1, $t1, 1
#     v
sw   $t1, 4($s2)
```

- Anti Abhängigkeit

    - i ließt und j schreibt

- Ausgabe Abhängigkeit

    - beide schreiben

> Data Hazards treten auf, wenn Abhängigkeiten zu **dicht** auf einander folgen.

#|RAW|WAW|WAR|RAR
:---:|:---:|:---:|:---:|:---:
Hazard?|ja|ja|ja|nein
Typ|echte Abh.|Ausgabeabh.|Anti Abh.|-
Pipeline?|ja|maybe|beim umsortieren|-

> NOPs einfügen

> Operand forwarding

![](https://www.bwuah.me/RO/c7.PNG)

![](https://www.bwuah.me/RO/c8.PNG)

> LW, dann Register lesen verursacht immer noch Hazard!

> STALL oder besser: Umordnen von Code

---

## Dynamisches Pipeline Scheduling

In Order falls möglich, sonst queue

-> WAR und WAW Hazards


## Kontrollflusskonflikte

> bei branches: wo steht der nächste Befehl

- Flushing: ???

# Branch Prediction

### Dynamische Sprungvorhersage:

> Vorhersage muss zwei mal falsch sein, damit übernommen wird.

> Branch Target Buffer (wie Cache)

# Superskalare MIPS

ALU|Datatransfere
:---:|:---:
-|lw
addi|-
add|-
bne|sw

# Caching

- zeitliche Lokalität

    - Schleifen/Unterprogramme

- räumliche Lokalität

    - Arrays/Objekte

---

- Hit vs Miss

- hit rate

- hit time

- miss penalty

--- 

## Direct Mapped Cache

![](https://www.bwuah.me/RO/c2.PNG)

![](https://www.bwuah.me/RO/c3.PNG)

### Lesen

hit|miss
:---:|:---:
easy|stall, schreiben, erneut lesen|

### Schreiben

-> Daten inkonsistent mit Hauptspeicher

Write Through | Write Back
:---:|:---:
gleichzeitig schreiben|dirty bit
-* |  read miss

\*mit Buffer -> read miss & buffer nicht leer

![](https://www.bwuah.me/RO/c4.PNG)

## Cache Types

![](https://www.bwuah.me/RO/c5.PNG)

## Fully Associative

- Komperator für jeden Eintrag

- wenig effizient

## Set Associative / n-way set-associative

![](https://www.bwuah.me/RO/c6.PNG)

### Verdrängungsstrategien

- RANDOM

- LIFO

- FIFO

- LFU *least frequently used*

- LRU *least recently used*

