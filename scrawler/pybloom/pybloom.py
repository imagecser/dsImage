#! /usr/bin/python3
# coding: utf-8

from bitstring import BitArray
import math
import pybloom.hashf as hashf


class BloomFilter:
    def __init__(self, n=100, error=0.001):
        self.n = n
        if not n > 0:
            raise ValueError("BloomFilter: n must be > 0.")
        if not (0 < error < 1):
            raise ValueError("error must be between 0 and 1.")
        self.error = error
        self.capacity = int(- self.n * math.log(self.error) / (math.log(2) ** 2)) + 1
        self.k = int(- math.log(self.error, 2)) + 1
        self.count = 0
        self.hashes = []
        for i in range(10):
            self.hashes.append(getattr(hashf, "hashf" + str(i)))
        self.hashes.append(getattr(hashf, "hashf"))
        self.bits = BitArray(int=0, length=self.capacity)

    def add(self, key):
        key = str(key)
        # if self.count > self.capacity:
        #     raise IndexError("BloomFilter is at capacity.")
        if self.count > self.n:
            raise IndexError("BloomFilter is overload.")
            # pass
        all_found = True
        offset = 0
        for i in range(self.k):
            if i > 9:
                value = self.hashes[10](key, self.capacity, i % 10)
            else:
                value = self.hashes[i](key, self.capacity)
            if all_found and not self.bits[value]:
                all_found = False
            self.bits[offset + value] = True
        if not all_found:
            self.count += 1
            return True
        else:
            return False

    def __contains__(self, key):
        key = str(key)
        for i in range(self.k):
            if i > 9:
                value = self.hashes[10](key, self.capacity, i % 10)
            else:
                value = self.hashes[i](key, self.capacity)
            if not self.bits[value]:
                return False
        return True
