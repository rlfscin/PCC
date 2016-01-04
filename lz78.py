class dnode(object):
	def __init__(self, index):
		self.index = index
		self.dchd = {}
	def haschd(self, c):
		return c in self.dchd
	def chd(self, c):
		return self.dchd[c]
	def addchd(self, c, node):
		assert c not in self.dchd
		self.dchd[c] = node

def inttostr(n, alphabet):
	if n==0:
		return alphabet[0]

	r = len(alphabet)
	ret  = ""
	while n > 0:
		ret = alphabet[n%r] + ret
		n = n/r
	return ret

def _rev_encode(code, alphabet):
	n = len(code)
	return code if n==1 else _rev_encode(inttostr(n-2, alphabet), alphabet) + code

def rev_encode(code, alphabet):
	return _rev_encode(alphabet[1] + code, alphabet) + alphabet[0]

def cw_encode(p, c , inputAB, outputAB):
	return rev_encode(inttostr(p, outputAB), outputAB) + rev_encode(inttostr(inputAB.index(c), outputAB), outputAB)

def lz78_encode(txt, inputAB, outputAB):
	txt = txt + "$"
	n = len(txt)
	D = dnode(0)
	d = 1
	code = ""
	i = 0
	X = ""
	cur = D

	while i < n:
		if cur.haschd(txt[i]):
			cur = cur.chd(txt[i])
			X = X + txt[i]
		else:
			code += cw_encode(cur.index, txt[i], inputAB, outputAB)
			cur.addchd(txt[i], dnode(d))
			d += 1
			X = ""
			cur = D
		i += 1
	return code

def strtoint(s, alphabet):
	r = len(alphabet)
	rpow = 1
	n = 0

	for J in range(len(s)-1, -1, -1):
		n += alphabet.index(s[J]) * rpow
		rpow *= r
	return n

def cw_decode(code, alphabet):
	k = 1
	J = 0

	while J == 0 or code[J] != alphabet[0]:
		Y = code[J:J+k]
		J += k
		k = strtoint(Y, alphabet) + 2
	return (strtoint(Y[1:], alphabet), J + 1)


def lz78_decode(code, inputAB, outputAB):
	txt = ""
	D = [(0,0)]
	i = 0
	n = len(code)

	while i < n:
		(p, l) = cw_decode(code[i:], outputAB)
		i += l
		(k, l) = cw_decode(code[i:], outputAB)
		i += l
		txt += (txt[D[p][0]:D[p][1]] + inputAB[k])
		D.append((len(txt)-((D[p][1]-D[p][0])+1), len(txt)))
	return txt[:len(txt)-1]

def main():
	# print "12", inttostr(12, "01")
	# code = inttostr(12, "01")
	# print code, "=", strtoint(code, "01")
	# rcode = rev_encode(code, "01")
	# print "rcode=", rcode
	# (p, l) = cw_decode(rcode, "01")
	# print "p=", p, "l=", l

	txt = "aabcbcbcbacbabcbabccbabb"
	A = list("abc$")
	E = list("01")
	code = lz78_encode(txt, A, E)
	print code

	decode = lz78_decode(code, A, E)
	print decode

if __name__ == '__main__':
	main()