import math

def code_int(n, alphabet, nbits):
	code = ""
	r = len(alphabet)

	while(n):
		bit = n%r
		code = alphabet[bit] + code
		n = n/r
	code = (nbits - len(code)) * alphabet[0] + code
	
	return code

def decode_int(code, alphabet, nbits):
	n = 0
	r = len(alphabet)
	rpow = 1
	p = len(code) - 1
	while(p >= 0):
		bit = code[p]
		n += (alphabet.index(bit)*rpow)
		p -= 1
		rpow *= r
	return n

def prefix_match(W, Ls):
	lmax = 0
	p = 0
	m = len(W)
	for s in range(Ls):
		l = 0
		while(Ls + l < m and W[s+l] == W[Ls + l]):
			l += 1
		if(l > lmax):
			lmax = l
			p = s
	return (p, lmax)

def lz77_encode(txt, Ls, Ll, alphabet):
	txt = txt + '$'
	code = ""
	r = len(alphabet)
	pbits = int(math.ceil(math.log(Ls, r)))
	lbits = int(math.ceil(math.log(Ll, r)))
	n = len(txt)
	W = Ls * "_" + txt[0:min(n, Ll)]
	
	J = -1
	while(len(W) > Ls):
		# print "window", W[0:Ls]+"."+W[Ls:]
		(p, l) = prefix_match(W, Ls)
		# print "p=", p, "l=", l
		c = txt[J+l+1]
		code += code_int(p, alphabet, pbits) + code_int(l, alphabet, lbits) + c
		W = W[l+1:] + txt[J+Ll+1:min(n, J+Ll+l+2)]
		J += (l+1)
	return code

def lz77_decode(code, Ls, Ll, alphabet):
	r = len(alphabet)
	pbits = int(math.ceil(math.log(Ls, r)))
	lbits = int(math.ceil(math.log(Ll, r)))
	i = 0
	n = len(code)
	txt = (Ls*"_")
	J = 0

	while(i < n):
		p = decode_int(code[i:i+pbits], alphabet, pbits)
		i += pbits
		l = decode_int(code[i:i+lbits], alphabet, lbits)
		i += lbits
		c = code[i]
		i += 1
		# print "p, l, c", p, l, c
		k = 0
		while(k < l):
			txt = txt + txt[J+p+k]
			k += 1
		txt += c
		J += (l+1)
	return txt[Ls:]

def main():
	txt = "abracadabra"
	# txt ="aababbabbac"
	txt = 20*txt
	alphabet = list('abcdr')
	# print txt
	code = lz77_encode(txt, 16, 16, alphabet)

	print "code=", code

	decode = lz77_decode(code, 16, 16, alphabet)

	print "decoded=", decode

	# a = code_int(24, alphabet, 2)

	# print a + "\n"

	# b = decode_int(a, alphabet, 2)

	# print b

if __name__ == '__main__':
	main()