use strict;

# Converts a "magic" string into slightly more obfuscated stuff

my $magic = <STDIN>;
chomp($magic);
die("You're not a magician\n") if ($magic =~ /^\s*$/);

print("static char magic[] = {\n");
my $counter = 131;
for my $ch (split('', $magic)) {
    printf("    %d,\n", ord($ch) + $counter);
    $counter++;
}
print("    0\n};\n");
