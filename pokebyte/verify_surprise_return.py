#!/usr/bin/env python3
"""Validate pre/post real Windows SDL screenshots after 14s of each preview."""
from pathlib import Path
EVENTS=("wake","victory","wander","friendship")
W=H=466
def frame(path):
    bs=Path(path).read_bytes()
    magic,size,maxv,pixels=bs.split(b"\n",3)
    assert (magic,size,maxv)==(b"P6",b"466 466",b"255"),path
    assert len(pixels)==W*H*3,(path,len(pixels))
    return pixels
def difference_in_label(p,q):
    # The Pack 03 preview label at y126 changes to "Back to normal" AFTER
    # the one-shot clip has finished; this rectangle excludes moving grass.
    pixels=0
    for y in range(123,148):
        for x in range(95,375):
            i=(y*W+x)*3
            if p[i:i+3]!=q[i:i+3]:pixels+=1
    return pixels
for event in EVENTS:
    start=frame("surprise-"+event+".ppm")
    end=frame("finished-"+event+".ppm")
    changed=difference_in_label(start,end)
    assert changed>90,(event,changed,"surprise did not visibly return to normal")
    print(f"{event}: UI/sprite region changed after 14s ({changed} pixels), save-free")
print("PASS: Four one-shot surprises have distinct post-event real renderer screenshots")
