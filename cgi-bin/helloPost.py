#!/usr/bin/python3

import sys

body = sys.stdin.read()

print("Content-Type: text/plain")
print()
print("BODY RECIBIDO:")
print(body)
