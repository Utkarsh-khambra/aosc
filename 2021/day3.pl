use warnings;
use strict;

open my $file_h, '<', "input/day3" or die "Cannot open file: $!";
my $line = <$file_h>;
chomp $line;
print length $line;
