use strict;

# Get major version of fe
open(my $if, "fe -V |") or die("can't run fe to get version id");
my $ver = <$if>;
chomp($ver);
$ver =~ s{\..*}{};

# Check that there is a reference file
if (! -f "reference/lorem-$ver.txt") {
    die <<"EOF";

Reference file lorem-$ver.txt is missing. Possibly because I did not create it
yet. To do so:

    cd test/reference
    export FE_KEY=lorem
    fe -t lorem-$ver.txt cp lorem.txt lorem-$ver.txt

EOF
}

# We have a reference file. Let's see if this version still encrypts correctly.
$ENV{FE_KEY} = 'lorem';

print("Transcrypting reference/lorem.txt into /tmp/$$ for fe version $ver\n");
run("fe -t /tmp/$$ cp reference/lorem.txt /tmp/$$");
print("Comparing /tmp/$$ with pre-existing reference/lorem-$ver.txt\n");
run("diff /tmp/$$ reference/lorem-$ver.txt");

# Check that decryption works
print("Decrypting /tmp/$$\n");
run("fe -f /tmp/$$");
print("Comparing plaintext /tmp/$$ with reference/lorem.txt\n");
run("diff /tmp/$$ reference/lorem.txt");

print("Version reference tests: OK\n");

sub run($) {
    my $cmd = shift;
    system($cmd) and die("reference.pl: $cmd failed\n");
}
