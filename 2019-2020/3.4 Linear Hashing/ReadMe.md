# Linear hashing

**status:** afgewerkt

```
tests:
searching 4 at blockindex 0
expected 4, got: 4
searching 34 at blockindex 2
searching 34 in restpage
expected -1, got -1
```

## Known issues
* This algoritm will throw an error when the restpage is full. Expending to a
second restpage isn't implemented.

* A key which hashes to 500 will cause problems because this is the address used by the restpage.