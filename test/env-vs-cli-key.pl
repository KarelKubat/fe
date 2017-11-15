use strict;

sub run($) {
    my $cmd = shift;
    print(">> $cmd\n");
    system($cmd) and die("env-vs-cli.pl: $cmd failed\n");    
}

# Create some texts under /tmp
run("cp env-vs-cli-key.pl /tmp/$$.plain");
run("cp env-vs-cli-key.pl /tmp/$$.1");
run("cp env-vs-cli-key.pl /tmp/$$.2");

# Transcrypt using environment or cli key
run("fe -k test -f /tmp/$$.1");
run("FE_KEY=test fe -f /tmp/$$.2");

# Files must be the same
run("diff /tmp/$$.1 /tmp/$$.2");

# Transcrypt back using environment or cli key
run("fe -k test -f /tmp/$$.1");
run("FE_KEY=test fe -f /tmp/$$.2");

# Files must be identical to plain
run("diff /tmp/$$.plain /tmp/$$.1");
run("diff /tmp/$$.plain /tmp/$$.2");

# All done
run("rm -f /tmp/$$");

