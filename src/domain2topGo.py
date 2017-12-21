#!/usr/bin/env python3

import argparse
import re


def read_pfam(input_file):
    gene2domain = {}
    # HSAL10024-RA    198    284    195    295 PF07686.16  V-set             Domain     3    97   109     37.6   1.9e-09   1 CL0011
    with open(input_file, "r") as in_file:
        for line in in_file:
            line = line.strip()
            if len(line) != 0:
                if line[0] != '#':
                    tokens = re.split(' +', line)
                    if tokens[0] in gene2domain:
                        gene2domain[tokens[0]].append(tokens[5].split(".")[0])
                    else:
                        gene2domain[tokens[0]] = [tokens[5].split(".")[0]]
    return gene2domain


def read_interpro(input_file):
    #HSAL14711-RA	c9f654d6bde50001a86ff8f12a038722	962	ProSitePatterns	PS00028	Zinc finger C2H2 type domain signature.	577	600	-	T	28-09-2017	IPR013087	Zinc finger C2H2-type	GO:0003676
    gene2go = {}
    with open(input_file, "r") as in_file:
        for line in in_file:
            line = line.strip()
            tokens = line.split("\t")
            if "GO:" in tokens[-1]:
                terms = tokens[-1].split("|")
                if tokens[0] in gene2go:
                    gene2go[tokens[0]] != set(terms)
                else:
                    gene2go[tokens[0]] = set(terms)
    return gene2go


def read_dama(input_file):
    # 2.4e-09	120	168	gnl|Acep_1.0|ACEP10001-PA	PF00067	0
    gene2domain = {}
    with open(input_file, "r") as in_file:
        for line in in_file:
            tokens = line.split("\t")
            if tokens[3] in gene2domain:
                gene2domain[tokens[3]].append(tokens[4])
            else:
                gene2domain[tokens[3]] = [tokens[4]]
    return gene2domain


def read_pfam2go(input_file):
    # Pfam:PF00001 7tm_1 > GO:G-protein coupled receptor activity ; GO:0004930
    pfam2go = {}
    with open(input_file, "r") as in_file:
        for line in in_file:
            if line[0] != '!':
                line = line.strip()
                tokens = re.split(' ', line)
                domain = tokens[0].split(":")[1]
                if domain in pfam2go:
                    pfam2go[domain].append(tokens[-1])
                else:
                    pfam2go[domain] = [tokens[-1]]
    return pfam2go


def merge_domain_and_go(gene2domain, pfam2go):
    gene2go = {}
    for k,v in gene2domain.items():
        go_terms = set()
        for domain in v:
            if domain in pfam2go:
                go_terms |= set(pfam2go[domain])
        if len(go_terms) != 0:
            gene2go[k] = go_terms
    return gene2go


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--pfam","-p", help="A domain annotation file in pfam format")
    parser.add_argument("--interpro", "-i", help="A domain annotation file in interpro format")
    parser.add_argument("--dama", "-d", help="A domain annotation file in dama format")
    parser.add_argument("--pfam2go", "-g", help="The pfam2go file")
    parser.add_argument("--outfile", "-o", help="The output file")
    args = parser.parse_args()

    if args.pfam2go is not None:
        pfam2go = read_pfam2go(args.pfam2go)

    if args.pfam is not None:
        gene2domain = read_pfam(args.pfam)
        gene2go = merge_domain_and_go(gene2domain, pfam2go)

    if args.dama is not None:
        gene2domain = read_dama(args.dama)
        gene2go = merge_domain_and_go(gene2domain, pfam2go)

    if args.interpro is not None:
        gene2go = read_interpro(args.interpro)

    with open(args.outfile, "w") as out_file:
        for k,v in gene2go.items():
            out_file.write(k + "\t" + ",".join(v) + "\n")

if __name__ == "__main__":
    main()