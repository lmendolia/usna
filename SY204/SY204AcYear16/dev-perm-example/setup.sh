#!/bin/bash

# Course: SY204 - Systems Programming and Operating System Fundamentals
# Author: LCDR C. W. Hoffmeister
# Description: bash script to setup permissions for File Systems - Attributes (Part 2) in-class activities

# main function
if [ -d dir-g-r ] ; then  # Target exists
	chmod g=r dir-g-r
fi
if [ -f dir-g-r/howdy1.txt ] ; then  # Target exists
	chmod g=r dir-g-r/howdy1.txt
fi
if [ -f dir-g-r/howdy2.txt ] ; then  # Target exists
	chmod g=rw dir-g-r/howdy2.txt
fi
if [ -f dir-g-r/howdy3.txt ] ; then  # Target exists
	chmod g=w dir-g-r/howdy3.txt
fi
if [ -d dir-g-r/sub-dir-rx ] ; then  # Target exists
	chmod g=rx dir-g-r/sub-dir-rx
fi
if [ -f dir-g-r/sub-dir-rx/howdy1.txt ] ; then  # Target exists
	chmod g=r dir-g-r/sub-dir-rx/howdy1.txt
fi
if [ -f dir-g-r/sub-dir-rx/howdy2.txt ] ; then  # Target exists
	chmod g=rw dir-g-r/sub-dir-rx/howdy2.txt
fi
if [ -f dir-g-r/sub-dir-rx/howdy3.txt ] ; then  # Target exists
	chmod g=w dir-g-r/sub-dir-rx/howdy3.txt
fi

if [ -d dir-g-rw ] ; then  # Target exists
	chmod g=rw dir-g-rw
fi
if [ -f dir-g-rw/howdy1.txt ] ; then  # Target exists
	chmod g=r dir-g-rw/howdy1.txt
fi
if [ -f dir-g-rw/howdy2.txt ] ; then  # Target exists
	chmod g=rw dir-g-rw/howdy2.txt
fi
if [ -f dir-g-rw/howdy3.txt ] ; then  # Target exists
	chmod g=w dir-g-rw/howdy3.txt
fi

if [ -d dir-g-x ] ; then  # Target exists
	chmod g=x dir-g-x
fi
if [ -f dir-g-x/howdy1.txt ] ; then  # Target exists
	chmod g=r dir-g-x/howdy1.txt
fi
if [ -f dir-g-x/howdy2.txt ] ; then  # Target exists
	chmod g=rw dir-g-x/howdy2.txt
fi
if [ -f dir-g-x/howdy3.txt ] ; then  # Target exists
	chmod g=w dir-g-x/howdy3.txt
fi
if [ -d dir-g-x/sub-dir-rx ] ; then  # Target exists
	chmod g=rx dir-g-x/sub-dir-rx
fi
if [ -f dir-g-x/sub-dir-rx/howdy1.txt ] ; then  # Target exists
	chmod g=r dir-g-x/sub-dir-rx/howdy1.txt
fi
if [ -f dir-g-x/sub-dir-rx/howdy2.txt ] ; then  # Target exists
	chmod g=rw dir-g-x/sub-dir-rx/howdy2.txt
fi
if [ -f dir-g-x/sub-dir-rx/howdy3.txt ] ; then  # Target exists
	chmod g=w dir-g-x/sub-dir-rx/howdy3.txt
fi

if [ -d dir-g-rx ] ; then  # Target exists
	chmod g=rx dir-g-rx
fi
if [ -f dir-g-rx/howdy1.txt ] ; then  # Target exists
	chmod g=r dir-g-rx/howdy1.txt
fi
if [ -f dir-g-rx/howdy2.txt ] ; then  # Target exists
	chmod g=rw dir-g-rx/howdy2.txt
fi
if [ -f dir-g-rx/howdy3.txt ] ; then  # Target exists
	chmod g=w dir-g-rx/howdy3.txt
fi
if [ -d dir-g-rx/sub-dir-rx ] ; then  # Target exists
	chmod g=rx dir-g-rx/sub-dir-rx
fi
if [ -f dir-g-rx/sub-dir-rx/howdy1.txt ] ; then  # Target exists
	chmod g=r dir-g-rx/sub-dir-rx/howdy1.txt
fi
if [ -f dir-g-rx/sub-dir-rx/howdy2.txt ] ; then  # Target exists
	chmod g=rw dir-g-rx/sub-dir-rx/howdy2.txt
fi
if [ -f dir-g-rx/sub-dir-rx/howdy3.txt ] ; then  # Target exists
	chmod g=w dir-g-rx/sub-dir-rx/howdy3.txt
fi

if [ -d dir-g-rwx ] ; then  # Target exists
	chmod g=rwx dir-g-rwx
fi
if [ -f dir-g-rwx/howdy1.txt ] ; then  # Target exists
	chmod g=r dir-g-rwx/howdy1.txt
fi
if [ -f dir-g-rwx/howdy2.txt ] ; then  # Target exists
	chmod g=rw dir-g-rwx/howdy2.txt
fi
if [ -f dir-g-rwx/howdy3.txt ] ; then  # Target exists
	chmod g=w dir-g-rwx/howdy3.txt
fi

if [ -f howdy1.txt ] ; then  # Target exists
	chmod g=rw,o=rw howdy1.txt
fi
if [ -f howdy2.txt ] ; then  # Target exists
	chmod u-rwx,g=rw,o=rw howdy2.txt
fi
if [ -f howdy3.txt ] ; then  # Target exists
	chmod u-rwx,g-rwx,o=rw howdy3.txt
fi
if [ -f howdy4.txt ] ; then  # Target exists
	chmod u-rwx,g-rwx,o-rwx howdy4.txt
fi