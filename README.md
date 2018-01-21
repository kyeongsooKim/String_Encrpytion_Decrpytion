# Encrpytion & Decrpytion program
### Kyeongsoo Kim, August 2017 ~ September 2017


## Introduction
This program is a command line utility to encrypt and
decrypt strings and files using two different cipher scheme. The first encryption scheme is a Polybius cipher and the second is a Fractionated Morse cipher. The concept of the project is designed by Prof. Eugene in Stony Brook University, However all the actual code except base header file is made by me.


<pre>
usage: ./hw1 -h [any other number or type of arguments]
usage: ./hw1 [-h] -p|-f -e|-d [-k KEY] [-r ROWS] [-c COLUMNS]
    -h       Display this help menu.
    -p       Polybius Cipher
                 -e (Positional) - Encrypt using the Polybius cipher
                 -d (Positional) - Decrypt using the Polybius cipher
                 -k (Optional) - KEY is the key to be used in the cipher.
                                 It must have no repeated characters and each character in the key must be in `polybius_alphabet` in `const.c`.
                 -r (Optional) - ROWS is the number of rows for the Polybius cipher table.
                                 Must be between 9 and 15, inclusive. Defaults to 10.
                 -c (Optional) - COLUMNS is the number of columns for the Polybius cipher table.
                                 Must be between 9 and 15, inclusive. Defaults to 10.

    -f       Fractionated Morse Cipher
                 -e (Positional) - Encrypt using the Fractionated Morse cipher
                 -d (Positional) - Decrypt using the Fractionated Morse cipher
                 -k (Optional) - KEY is the key to be used in the cipher.
                                 It must have no repeated characters and each character in the key must be in `fm_alphabet` in `const.c`.
</pre>


# Part I Polybius Cipher

## Background
 Polybius cipher uses an alphabet (i.e. list of characters) to construct a
table. `polybius_alphabet` is declared in `const.c`, which has
ASCII characters 0x21-0x7e (33-126), to fill the table. 

> I improved this encryption scheme by adding a key. The key consists of a
subset of the characters in the alphabet and cannot have repeated characters.
The characters in the key are placed first in the table and the remaining
alphabet characters are placed after.

### Sample Encryption Execution

<pre>
$ bin/hw1 -p -e -r 11 -c 9 -k kyeong
I want to live in the United States
51 96770493 9303 86849502 8404 938302 640484930281 629377930292

</pre>



### Sample Decryption Execution

<pre>
$ bin/ -p -d
51 96770493 9303 86849502 8404 938302 640484930281 629377930292
I want to live in the United States
$ echo $?
0
$
</pre>

# Part II Fractionated Morse Cipher

[Morse Code](https://en.wikipedia.org/wiki/Morse_code) is a standardized method of transmitting text information. The text is translated into various sequences of "dots" `.` and "dashes" `-`. [Fractionated Morse Cipher](http://practicalcryptography.com/ciphers/classical-era/fractionated-morse/) takes this translated text and ciphers it using a key back into text that can only be decoded using that same key.


`const.h` defines `char *morse_table[]` which is translation table when
translating between text and Morse Code.

The key for the cipher is a mixed alphabet determined by the `-k` argument. To
show this we will explain how we determine the key with the `-k` argument being
`ROUNDTABLE`. This `-k` argument will expand to the key being
`ROUNDTABLECFGHIJKMPQSVWXYZ`. The letters in the `-k` argument will be the start
of the key, and the rest of the key is filled out with the letters of the
alphabet that are not in the `-k` argument.

The program will read from the input file, translate the given text to Morse
Code, and then cipher the Morse Code translation to Fractionated Morse Cipher.
In our morse translation, we will be putting one `x`(0x78) between each
character and `xx` at the end of each word and will be treating a word as any
amount of characters followed by any amount of whitespace.

In the ciphering of the intermediate Morse translation, the program will cipher
groups of three characters until there are no more groups of three characters
left. This means that any leftover characters will be truncated off the end and
not ciphered.




### Sample Encryption Execution:
<pre>
$ cat rsrc/message.txt
DEFEND THE EAST
$ cat rsrc/message.txt | bin/hw1 -f -e -k ROUNDTABLE
ESOAVVLJRSSTRX
$
</pre>




### Sample Decyption Execution:


<pre>
$ cat rsrc/encoded_message.txt
ESOAVVLJRSSTRX
$ cat rsrc/encoded_message.txt | bin/hw1 -f -d -k ROUNDTABLE
DEFEND THE EAST
$
</pre>




# CF) Fractionated Morse Cipher Detail

Let's say we will be ciphering the text `I LOVE CS` with the key `HELP`.

We must first translate the text to Morse Code. **We will be using the
translation table as defined in the `const.c` we have provided.**

`I LOVE CS` will be translated to `..xx.-..x---x...-x.xx-.-.x...xx`

`I` is mapped to the Morse Code `..`(dot dot). We then put the word divider `xx` to indicate that this is the end of the word. Next we translate `LOVE` as `.-..x---x...-x.xx`. As you can see, `L` translates to `.-..` and `O` translates to `---`. In between these characters, we have the letter divider, which is a single `x`. This routine is done for the rest of the text. To end the entire sequence, we have a terminating word divider.

Next we will determine the full key using the input value `HELP`. When we expand
the key, it will become: `HELPABCDFGIJKMNOQRSTUVWXYZ`. This is because the full
key is filled out with the rest of the letters of the alphabet that were unused.

Using the key, we will cipher the Morse Code. To do this, we must first
associate the letters of the key with the Fractionated Table values. This is
done in the following fashion:

<pre>
H E L P A B C D F G I J K M N O Q R S T U V W X Y Z
. . . . . . . . . - - - - - - - - - x x x x x x x x
. . . - - - x x x . . . - - - x x x . . . - - - x x
. - x . - x . - x . - x . - x . - x . - x . - x . -
</pre>

The table shows each possible Morse Code triple cipher. `...` is ciphered to
`H`, `..-` is ciphered to `E`, `..x` is ciphered to `L`, and so on.

>  Take note that there is no `xxx` on the table!

We will split the Morse Code into triples, and then cipher the triples:

> **As a  visual aid, the character `|` will be used to separate the
> triples in this example.**

`..x | x.- | ..x| --- | x.. | .-x | .xx | -.- | .xx`

will be ciphered to:

`L | T | L | M | S | B | F | I | F`

or just

`LTLMSBFICL`

To decrypt our message, we do the same steps in reverse. First we must decipher
the encrypted message back into Morse Code. For this to properly be done, we
must decipher using the same key so that our table is the same as when we
encrypted the message. As a reminder, the table will be listed again:

<pre>
H E L P A B C D F G I J K M N O Q R S T U V W X Y Z
. . . . . . . . . - - - - - - - - - x x x x x x x x
. . . - - - x x x . . . - - - x x x . . . - - - x x
. - x . - x . - x . - x . - x . - x . - x . - x . -
</pre>

Using the key we will build the original Morse Code translated message from the
ciphered text, which is `..xx.-..x---x...-x.xx-.-.x...xx`

Next we must translate the Morse Code back into plain text. This step requires
translating all of the dot-and-dash combinations to their plain text
equivalents, and translating word dividers accordingly.

`..` will be translated to `I`. Since it is followed by a word divider, we will be adding a space after `I` to get `I `. Next, we translate `.-..` to `L`. Since it is simply followed by a letter divider we will ignore it and translate the next `---` to `O`. This routine is done for the rest of the phrase, and we are left with our original phrase `I LOVE CS`.