import heapq

def compute_freqs(txt):
    freqs = {}
    for i in range(len(txt)):
        freqs[txt[i]] = 1 if txt[i] not in freqs else freqs[txt[i]] + 1
    return freqs

def make_node(char, freq, left, right):
    return [freq, char, left, right]

def print_trie(root, level):
    if not root:
        return
    print_trie(root[2], level + 1)
    print ((level * "   ") + "c=\'"+root[1]+"\' f=" + str(root[0]))
    print_trie(root[3], level + 1)

def fill_hcode(hcode, root, codeword):
    if not root[2]:
        hcode[root[1]] =  codeword
    else:
        fill_hcode(hcode, root[2], codeword+"0")
        fill_hcode(hcode, root[3], codeword+"1")

def huff_encode(txt):
    n = len(txt) 
    freqs = compute_freqs(txt)
    print (freqs)
    pq = [make_node(a, freqs[a], None, None) for a in freqs]
    heapq.heapify(pq)
    print (pq)
    m = len(pq)
    for i in range(m-1):
        l = heapq.heappop(pq)
        r = heapq.heappop(pq)
        root = make_node(l[1] + r[1], l[0] + r[0], l, r)
        heapq.heappush(pq, root)
    print_trie(root, 0)
    hcode = {}
    fill_hcode(hcode, root, "")
    print (hcode)
    encoded = "".join([hcode[txt[i]] for i in range(n)])
    print (encoded)
    return (root, hcode, encoded)

def huff_decode(trie, encoded):
    txt = ""
    n = len(encoded)
    cur = trie
    for i in range(n):
        cur = cur[ 2 + int(encoded[i])]
        if not cur[2]:
            txt += cur[1]
            cur = trie
    return txt

def int_to_binstr(n, nbits):
    s = ""
    while n:
        if n % 2:
            s = "1"+s
        else:
            s = "0"+s
        n = n/2
    s = ((nbits - len(s))*"0")+s
    return s

def main():
    txt = "abracadabra"
    (trie, hcode, encoded) = huff_encode(txt)
    decoded = huff_decode(trie, encoded)
    print (decoded)
    


main()
