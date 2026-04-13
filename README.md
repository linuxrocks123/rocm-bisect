 rocm-bisect
Script to bisect ROCm failures

Usage:

- First, check out The Rock and make it build.

- Then, use a command like:

`git log --oneline 78348cc2f1b03f43b9421d775abd6aa73d07ee5a^..78348cc2f1b03f43b9421d775abd6aa73d07ee5a > gitlog.txt`

to generate a list of SHAs that span the known range between "test
passes" and "test fails."

- Finally, from The Rock's `build` directory, run a command like this:

`../../rocm-bisect/rocm-bisect.py 'ninja clean; ninja amd-llvm' 'ninja rocSOLVER' < ../gitlog.txt`

Standard input should be that list of SHAs you got earlier.  The first
argument is the command to clean and build LLVM.  In this example,
it's set to the default.  The second argument is the command to build
-- or build and run, if you're debugging a runtime failure -- the
testcase that passes with the oldest SHA and fails with the newest
SHA.

You can pass a third argument if you like.  That argument is the
directory where you need to go to apply the cherry-picked SHAs.  In
our case, the default `../compiler/amd-llvm` is correct.

Once the bisection is complete, output will be the SHAs of the latest
known good commit and the earliest known failing commit

Example bisect of a failure between two commits on amd-staging for LLVM:
```
cd ../compiler/amd-llvm

#Generate the list of commits between good and bad
git log --oneline --first-parent 6602f325c28907dba03c42fc0263c4a309529c52..c849bc16b0e49951d313756f20b73c2b28d321d7 > /work/scratch/gitlog_1.txt

#Reset branch
git reset --hard 6602f325

#Make sure all commits apply (they should since we're not inside a merge)
for i in `cat ../../build/gitlog_1.txt | sed 's/ .*//' | tac`; do git cherry-pick -m 1 $i; done

#Reset branch again
git reset --hard 6602f325

#Run script
cd ../../build
/work/scratch/rocm-bisect/rocm-bisect.py 'ninja clean; ninja amd-llvm' ninja ../compiler/amd-llvm "-m 1" < /work/scratch/gitlog_1.txt
```

Bisecting inside of a merge after finding it's the commit that's bad:
```
#Generate the list of commits between good and bad
pushd ../compiler/amd-llvm
git reset --hard 12345678
git log --oneline 12345678^..12345678 > /work/scratch/gitlog_2.txt

#Make sure all commits apply:
for i in `cat ../../build/gitlog_2.txt | sed 's/ .*//' | tac`; do git cherry-pick --allow-empty $i; done

#Fix the ones that don't

#Edit files
vi

#Commit fix
git commit -a

#Replace SHA in /work/scratch/gitlog_2.txt
vi /work/scratch/gitlog_2.txt

#Rerun test
git reset --hard 12345678
for i in `cat ../../build/gitlog_2.txt | sed 's/ .*//' | tac`; do git cherry-pick --allow-empty $i; done

#After it passes
git reset --hard 12345678
popd

#Run script
/work/scratch/rocm-bisect/rocm-bisect.py 'ninja clean; ninja amd-llvm' ninja ../compiler/amd-llvm "--allow-empty" < /work/scratch/gitlog_2.txt
```
