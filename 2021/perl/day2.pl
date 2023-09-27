use strict;
use warnings;

open my $file_h, '<', '../input/day2' or die "Cannot open day2 file: $!";

my $total_forward = 0;
my $total_depth   = 0;
my $part_2_depth  = 0;
my $aim           = 0;
while (my $line = <$file_h>)
{
    chomp $line;
    my ($command, $value) = split(' ', $line);
    if (substr($command, 0, 1) eq 'f')
    {
        $total_forward += $value;
        $part_2_depth  += $aim * $value;
    }
    if (substr($command, 0, 1) eq 'd')
    {
        $total_depth += $value;
        $aim         += $value;
    }
    if (substr($command, 0, 1) eq 'u')
    {
        $total_depth -= $value;
        $aim         -= $value;
    }
}
print $total_forward*$total_depth,  "\n";
print $total_forward*$part_2_depth, "\n";
