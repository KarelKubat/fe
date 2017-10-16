use strict;
use Time::HiRes qw(gettimeofday tv_interval);

# Prepare a large file
my $t0 = [gettimeofday()];
open(my $of, ">/tmp/large.txt") or die;
for my $i (1..10000) {
    for my $j (1..100) {
	print $of ("0123456789");
    }
}
close($of);
print("File creation        : ", tv_interval($t0), "sec\n");

# cp test
$t0 = [gettimeofday()];
system("cp /tmp/large.txt /tmp/large0.txt") and die;
print("Plain cp             : ", tv_interval($t0), "sec\n");

# Transcrypt inline
$t0 = [gettimeofday()];
system("fe -k secret -f /tmp/large.txt") and die;
print("Inline transcryption : ", tv_interval($t0), "sec\n");

open(my $if, "uname |") or die;
if (<$if> !~ /Darwin/) {
    # Transcrypt using cp
    $t0 = [gettimeofday()];
    system("fe -k secret -t /tmp/large.txt cp /tmp/large.txt /tmp/large1.txt")
      and die;
    print("Transcribing via cp  : ", tv_interval($t0), "sec\n");

    # Check
    system("diff /tmp/large0.txt /tmp/large1.txt")
      and die("encryption/decryption failed, ",
	      "/tmp/large.txt and /tmp/large1.txt differ\n");
}
