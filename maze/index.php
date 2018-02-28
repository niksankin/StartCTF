<?php

$vis = [];
$ver = [];
$hor = [];
$lines = 60;
//$address = '127.0.0.1';
$address = '0.0.0.0';
$port = 5500;
$iter = 1;

function index($num, $lines){
    if($num < 0)
        return $num < -$lines ? $lines + $num % $lines : $lines + $num;
    else
        return $num < $lines ? $num : $num % $lines; 
}

function walk(int $x, int $y, $lines, &$hor, &$ver, &$vis)
{
    $vis[$y][$x] = 1;

    $d = [[$x - 1, $y], [$x, $y + 1], [$x + 1, $y], [$x, $y - 1]];
    shuffle($d);
    foreach ($d as $point) {

        if($vis[index($point[1], $lines + 1)][index($point[0], $lines + 1)])
            continue;
        if($point[0] == $x)
            $hor[max($y, $point[1])][$x] = '* ';
        if($point[1] == $y)
            $ver[$y][max($x, $point[0])] = '  ';
        
        walk($point[0], $point[1], $lines, $hor, $ver, $vis);
    }
}

function create_Maze(int $lines, array $start, &$hor, &$ver, &$vis)
{

    for ($i = 0; $i < $lines + 1; ++$i) {
        if($i != $lines)
            $vis[] = array_merge(array_fill(0, $lines, 0), [1]);
        else
            $vis[] = array_fill(0, $lines + 1, 1);
        
        if($i != $lines)
            $ver[] = array_merge(array_fill(0, $lines, '* '), ['*']);
        else
            $ver[] = [];
        $hor[] = array_merge(array_fill(0, $lines, '**'), ['*']);
    }

    walk($start[0], $start[1], $lines, $hor, $ver, $vis);
    $s = [];

    for ($i = 0; $i < $lines + 1; ++$i){
        $s[] = $hor[$i];
        $s[] = $ver[$i];
    }

    return $s;
}

set_time_limit(0);

ob_implicit_flush();

if (($sock = socket_create(AF_INET, SOCK_STREAM, SOL_TCP)) === false) {
    echo "Не удалось выполнить socket_create(): причина: " . socket_strerror(socket_last_error()) . "\n";
}

if (socket_bind($sock, $address, $port) === false) {
    echo "Не удалось выполнить socket_bind(): причина: " . socket_strerror(socket_last_error($sock)) . "\n";
}

if (socket_listen($sock, 5) === false) {
    echo "Не удалось выполнить socket_listen(): причина: " . socket_strerror(socket_last_error($sock)) . "\n";
}

while(1){
    $recv = socket_accept($sock);

    socket_write($recv, 'NO CHEATING OR YOU DIE'."\n");

    for($i = 0; $i < $iter; ++$i){
        $maze = create_Maze($lines, [0, 0], $hor, $ver, $vis);
        $data = '';

        foreach($maze as $line)
            $data.=implode('', $line) . "\n";
        $data[($lines + 1)*2 + 1] = 's';
        $data[strlen($data) - ($lines + 3)*2] = 'f';
        print($data);

        socket_write($recv, $data, strlen($data));

        $input = str_split(socket_read($recv, 16392));
        $step = [1,1];
        $error = '';
		$data = str_replace("\n", '', $data);

        foreach($input as $command){
    		print($command);
            if($command === 'u')
                $step[1] -= 1;
            else if($command === 'd')
                $step[1] += 1;
            else if($command === 'l')
                $step[0] -= 1;
            else if($command === 'r')
                $step[0] += 1;

            if($data[$step[1] * ($lines * 2 + 1) + $step[0]] !== ' ' and $data[$step[1] * ($lines * 2 + 1) + $step[0]] !== 'f'){
                $error = 'Wrong way!'."\n";
                socket_write($recv, $error, strlen($error));
                break;
            }
        }

        if(!empty($error))
            break;
        else if($data[$step[1] * ($lines * 2 + 1) + $step[0]] !== 'f'){
            $error = 'You must reach the end of the maze to save Hot Dog Kinghts!'."\n";
            socket_write($recv, $error, strlen($error));
            break;
        }

        if($i === $iter - 1)
            socket_write($recv, "Congratulation, warrior! Come and get what you wish: MIEM{n0w_15_7h3_4m4z1n6_71m3_f0r_4dv3n7ur35!}");
    }

    socket_close($recv);
}

socket_close($sock);
