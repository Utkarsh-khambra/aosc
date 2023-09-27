use strict;
use warnings;

open my $input_fh, '<', "../input/day1"
  or die "Cannot open input data file: $!";

sub sum($)
{
    my $list = shift;
    my $sum  = 0;
    foreach my $i (@$list)
    {
        $sum += $i;
    }
    return $sum;
}

my $count = 0;
my @nums;
while (my $num = <$input_fh>)
{
    chomp $num;
    push @nums, $num;
}
close $input_fh;

foreach my $i (0 .. $#nums - 1)
{
    my ($first, $second) = @nums[$i .. $i + 1];
    $count += $first < $second;
}

print $count , "\n";
$count = 0;
my @nums_3;
foreach my $i (0 .. $#nums - 2)
{
    my @window = @nums[$i .. $i + 2];

    # print join ',', @window , "\n";
    push @nums_3, sum(\@window);
}

foreach my $i (0 .. $#nums_3 - 1)
{
    my @window = @nums_3[$i .. $i + 2];
    $count += $window[0] < $window[1];
}

print $count;
