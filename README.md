# rocm-bisect
Script to bisect ROCm failures

Usage:

- First, check out The Rock and make it build.

- Then, use a command like:

`git log --oneline 78348cc2f1b03f43b9421d775abd6aa73d07ee5a..0c4f8094939d2a2b50b6cd062cd1473a0315457f > gitlog.txt`

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

Once the bisection is complese, output will be the SHAs of the latest
known good commit and the earliest known failing commit
