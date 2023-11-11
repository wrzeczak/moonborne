# Politics

---

### The Basics

As of now, politics are graded using the [8values](https://8values.github.io) scale, with four axes stretching between two extremes. I chose to represent every axis as a single floating point value, with each extreme being either a one or a zero.

If we have an example test result like this:

```
Equality    ########--    Markets    (Economic Score of 0.8f)
Nation      ####------    Peaceful   (Diplomatic Score of 0.4f)
Liberal     ######----    Authority  (Civil Score of 0.6f)
Tradition   ####------    Progress   (Societal Score of 0.4f)
```

Scores are measured as the percent score on the left side; the 8values test gives percentages for both sides, but since getting one from the other is trivial I chose to include only one side for ease of use. Note that the interactive character creator does not include an 8values test, it simply links you to one beforehand and asks you to input values manually.
