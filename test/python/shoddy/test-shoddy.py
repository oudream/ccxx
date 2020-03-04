import shoddy

s = shoddy.Shoddy1(range(3))
s.extend(s)
print(len(s))

print(s.increment())

print(s.increment())
