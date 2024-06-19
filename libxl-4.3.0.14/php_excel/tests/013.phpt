--TEST--
Border Colors
--INI--
date.timezone=America/Toronto
--SKIPIF--
<?php if (!extension_loaded("excel")) print "skip"; ?>
--FILE--
<?php 
	$x = new ExcelBook();

	$s = $x->addSheet("Sheet 1");

	$data = "Test";
	$oClass = new ReflectionClass('ExcelFormat');

	$row = $col = 0;

	foreach (array('', 'Left','Right','Top','Bottom') as $b) {
		foreach ($oClass->getConstants() as $c => $val) {
			if (strpos($c, 'COLOR_') !== 0) {
				continue;
			}

			$f = 'border' . $b . 'Color';

			$format = $x->addFormat();
			$format->$f($val);
			$format->borderStyle(ExcelFormat::BORDERSTYLE_THICK);

			var_dump($s->write($row, 1, $data, $format));
			var_dump($x->getError());

			$fmt = null;
			$s->read($row, 1, $fmt);
			echo $fmt->$f() . "\n";

			$row += 2;
		}
	}




	echo "OK\n";
?>
--EXPECT--
bool(true)
bool(false)
1
bool(true)
bool(false)
1
bool(true)
bool(false)
1
bool(true)
bool(false)
1
bool(true)
bool(false)
1
bool(true)
bool(false)
1
bool(true)
bool(false)
1
bool(true)
bool(false)
1
bool(true)
bool(false)
1
bool(true)
bool(false)
1
bool(true)
bool(false)
1
bool(true)
bool(false)
1
bool(true)
bool(false)
1
bool(true)
bool(false)
1
bool(true)
bool(false)
1
bool(true)
bool(false)
1
bool(true)
bool(false)
1
bool(true)
bool(false)
1
bool(true)
bool(false)
1
bool(true)
bool(false)
1
bool(true)
bool(false)
1
bool(true)
bool(false)
1
bool(true)
bool(false)
1
bool(true)
bool(false)
1
bool(true)
bool(false)
1
bool(true)
bool(false)
1
bool(true)
bool(false)
1
bool(true)
bool(false)
1
bool(true)
bool(false)
1
bool(true)
bool(false)
1
bool(true)
bool(false)
1
bool(true)
bool(false)
1
bool(true)
bool(false)
1
bool(true)
bool(false)
1
bool(true)
bool(false)
1
bool(true)
bool(false)
1
bool(true)
bool(false)
1
bool(true)
bool(false)
1
bool(true)
bool(false)
1
bool(true)
bool(false)
1
bool(true)
bool(false)
1
bool(true)
bool(false)
1
bool(true)
bool(false)
1
bool(true)
bool(false)
1
bool(true)
bool(false)
1
bool(true)
bool(false)
1
bool(true)
bool(false)
1
bool(true)
bool(false)
1
bool(true)
bool(false)
1
bool(true)
bool(false)
1
bool(true)
bool(false)
1
bool(true)
bool(false)
1
bool(true)
bool(false)
1
bool(true)
bool(false)
1
bool(true)
bool(false)
1
bool(true)
bool(false)
1
bool(true)
bool(false)
1
bool(true)
bool(false)
1
bool(true)
bool(false)
8
bool(true)
bool(false)
9
bool(true)
bool(false)
10
bool(true)
bool(false)
11
bool(true)
bool(false)
12
bool(true)
bool(false)
13
bool(true)
bool(false)
14
bool(true)
bool(false)
15
bool(true)
bool(false)
16
bool(true)
bool(false)
17
bool(true)
bool(false)
18
bool(true)
bool(false)
19
bool(true)
bool(false)
20
bool(true)
bool(false)
21
bool(true)
bool(false)
22
bool(true)
bool(false)
23
bool(true)
bool(false)
24
bool(true)
bool(false)
25
bool(true)
bool(false)
26
bool(true)
bool(false)
27
bool(true)
bool(false)
28
bool(true)
bool(false)
29
bool(true)
bool(false)
30
bool(true)
bool(false)
31
bool(true)
bool(false)
32
bool(true)
bool(false)
33
bool(true)
bool(false)
34
bool(true)
bool(false)
35
bool(true)
bool(false)
36
bool(true)
bool(false)
37
bool(true)
bool(false)
38
bool(true)
bool(false)
39
bool(true)
bool(false)
40
bool(true)
bool(false)
41
bool(true)
bool(false)
42
bool(true)
bool(false)
43
bool(true)
bool(false)
44
bool(true)
bool(false)
45
bool(true)
bool(false)
46
bool(true)
bool(false)
47
bool(true)
bool(false)
48
bool(true)
bool(false)
49
bool(true)
bool(false)
50
bool(true)
bool(false)
51
bool(true)
bool(false)
52
bool(true)
bool(false)
53
bool(true)
bool(false)
54
bool(true)
bool(false)
55
bool(true)
bool(false)
56
bool(true)
bool(false)
57
bool(true)
bool(false)
58
bool(true)
bool(false)
59
bool(true)
bool(false)
60
bool(true)
bool(false)
61
bool(true)
bool(false)
62
bool(true)
bool(false)
63
bool(true)
bool(false)
64
bool(true)
bool(false)
65
bool(true)
bool(false)
8
bool(true)
bool(false)
9
bool(true)
bool(false)
10
bool(true)
bool(false)
11
bool(true)
bool(false)
12
bool(true)
bool(false)
13
bool(true)
bool(false)
14
bool(true)
bool(false)
15
bool(true)
bool(false)
16
bool(true)
bool(false)
17
bool(true)
bool(false)
18
bool(true)
bool(false)
19
bool(true)
bool(false)
20
bool(true)
bool(false)
21
bool(true)
bool(false)
22
bool(true)
bool(false)
23
bool(true)
bool(false)
24
bool(true)
bool(false)
25
bool(true)
bool(false)
26
bool(true)
bool(false)
27
bool(true)
bool(false)
28
bool(true)
bool(false)
29
bool(true)
bool(false)
30
bool(true)
bool(false)
31
bool(true)
bool(false)
32
bool(true)
bool(false)
33
bool(true)
bool(false)
34
bool(true)
bool(false)
35
bool(true)
bool(false)
36
bool(true)
bool(false)
37
bool(true)
bool(false)
38
bool(true)
bool(false)
39
bool(true)
bool(false)
40
bool(true)
bool(false)
41
bool(true)
bool(false)
42
bool(true)
bool(false)
43
bool(true)
bool(false)
44
bool(true)
bool(false)
45
bool(true)
bool(false)
46
bool(true)
bool(false)
47
bool(true)
bool(false)
48
bool(true)
bool(false)
49
bool(true)
bool(false)
50
bool(true)
bool(false)
51
bool(true)
bool(false)
52
bool(true)
bool(false)
53
bool(true)
bool(false)
54
bool(true)
bool(false)
55
bool(true)
bool(false)
56
bool(true)
bool(false)
57
bool(true)
bool(false)
58
bool(true)
bool(false)
59
bool(true)
bool(false)
60
bool(true)
bool(false)
61
bool(true)
bool(false)
62
bool(true)
bool(false)
63
bool(true)
bool(false)
64
bool(true)
bool(false)
65
bool(true)
bool(false)
8
bool(true)
bool(false)
9
bool(true)
bool(false)
10
bool(true)
bool(false)
11
bool(true)
bool(false)
12
bool(true)
bool(false)
13
bool(true)
bool(false)
14
bool(true)
bool(false)
15
bool(true)
bool(false)
16
bool(true)
bool(false)
17
bool(true)
bool(false)
18
bool(true)
bool(false)
19
bool(true)
bool(false)
20
bool(true)
bool(false)
21
bool(true)
bool(false)
22
bool(true)
bool(false)
23
bool(true)
bool(false)
24
bool(true)
bool(false)
25
bool(true)
bool(false)
26
bool(true)
bool(false)
27
bool(true)
bool(false)
28
bool(true)
bool(false)
29
bool(true)
bool(false)
30
bool(true)
bool(false)
31
bool(true)
bool(false)
32
bool(true)
bool(false)
33
bool(true)
bool(false)
34
bool(true)
bool(false)
35
bool(true)
bool(false)
36
bool(true)
bool(false)
37
bool(true)
bool(false)
38
bool(true)
bool(false)
39
bool(true)
bool(false)
40
bool(true)
bool(false)
41
bool(true)
bool(false)
42
bool(true)
bool(false)
43
bool(true)
bool(false)
44
bool(true)
bool(false)
45
bool(true)
bool(false)
46
bool(true)
bool(false)
47
bool(true)
bool(false)
48
bool(true)
bool(false)
49
bool(true)
bool(false)
50
bool(true)
bool(false)
51
bool(true)
bool(false)
52
bool(true)
bool(false)
53
bool(true)
bool(false)
54
bool(true)
bool(false)
55
bool(true)
bool(false)
56
bool(true)
bool(false)
57
bool(true)
bool(false)
58
bool(true)
bool(false)
59
bool(true)
bool(false)
60
bool(true)
bool(false)
61
bool(true)
bool(false)
62
bool(true)
bool(false)
63
bool(true)
bool(false)
64
bool(true)
bool(false)
65
bool(true)
bool(false)
8
bool(true)
bool(false)
9
bool(true)
bool(false)
10
bool(true)
bool(false)
11
bool(true)
bool(false)
12
bool(true)
bool(false)
13
bool(true)
bool(false)
14
bool(true)
bool(false)
15
bool(true)
bool(false)
16
bool(true)
bool(false)
17
bool(true)
bool(false)
18
bool(true)
bool(false)
19
bool(true)
bool(false)
20
bool(true)
bool(false)
21
bool(true)
bool(false)
22
bool(true)
bool(false)
23
bool(true)
bool(false)
24
bool(true)
bool(false)
25
bool(true)
bool(false)
26
bool(true)
bool(false)
27
bool(true)
bool(false)
28
bool(true)
bool(false)
29
bool(true)
bool(false)
30
bool(true)
bool(false)
31
bool(true)
bool(false)
32
bool(true)
bool(false)
33
bool(true)
bool(false)
34
bool(true)
bool(false)
35
bool(true)
bool(false)
36
bool(true)
bool(false)
37
bool(true)
bool(false)
38
bool(true)
bool(false)
39
bool(true)
bool(false)
40
bool(true)
bool(false)
41
bool(true)
bool(false)
42
bool(true)
bool(false)
43
bool(true)
bool(false)
44
bool(true)
bool(false)
45
bool(true)
bool(false)
46
bool(true)
bool(false)
47
bool(true)
bool(false)
48
bool(true)
bool(false)
49
bool(true)
bool(false)
50
bool(true)
bool(false)
51
bool(true)
bool(false)
52
bool(true)
bool(false)
53
bool(true)
bool(false)
54
bool(true)
bool(false)
55
bool(true)
bool(false)
56
bool(true)
bool(false)
57
bool(true)
bool(false)
58
bool(true)
bool(false)
59
bool(true)
bool(false)
60
bool(true)
bool(false)
61
bool(true)
bool(false)
62
bool(true)
bool(false)
63
bool(true)
bool(false)
64
bool(true)
bool(false)
65
OK
