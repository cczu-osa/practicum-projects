<?php
    class Md5Encrypt {
		const A = 0x67452301;
		const B = 0xEFCDAB89;
		const C = 0x98BADCFE;
		const D = 0x10325476;
		private $file;
		private $filelength;
		private $filepath;
        function __construct(){
            echo '<br>Start: <br>';
        }
        function __destruct(){
            echo '<br>destruct<br>';
        }
		public function left($input, $n){
			$right = $input >> (32-$n);
			$left = ($input << $n) - ($right << 32);
			return $left ^ $right;
		}
        public function fix($input){
			$temp = $input;
			$a = ($temp >> 24);
			$b = ($temp >> 16) 	- ($a << 8);
			$c = ($temp >> 8) 	- ($a << 16) - ($b << 8);
			$d = ($temp) 		- ($a << 24) - ($b << 16) - ($c << 8);
			return ($d << 24) + ($c << 16) + ($b << 8) + $a;
		}
		public function f($x,$y,$z){
			return ($x & $y) | ((0xffffffff - $x) & $z);
		}
		public function g($x,$y,$z){
			return ($x & $z) | ($y & (0xffffffff - $z));
		}
		public function h($x,$y,$z){
			return $x ^ $y ^ $z;
		}
		public function i($x,$y,$z){
			return $y ^ ($x | (0xffffffff - $z));
		}
		public function ff(&$a, $b, $c, $d, $M, $s, $ti){
			$a += $this->f($b, $c, $d) + $M + $ti;
			if($a > 0x300000000){
					$a = $a - 0x300000000;
				}else
				if($a > 0x200000000){
					$a = $a - 0x200000000;
				}else
				if($a > 0x100000000){
					$a = $a - 0x100000000;
			}
			$a = $this->left($a, $s);
			$a = $a + $b;
			if($a > 0x100000000){
				$a = $a - 0x100000000;
			}
		}
		public function gg(&$a, $b, $c, $d, $M, $s, $ti){
			$a += $this->g($b, $c, $d) + $M + $ti;
			if($a > 0x300000000){
					$a = $a - 0x300000000;
				}else
				if($a > 0x200000000){
					$a = $a - 0x200000000;
				}else
				if($a > 0x100000000){
					$a = $a - 0x100000000;
			}
			$a = $this->left($a, $s);
			$a = $a + $b;
			if($a > 0x100000000){
				$a = $a - 0x100000000;
			}
		}
		public function hh(&$a, $b, $c, $d, $M, $s, $ti){
			$a += $this->h($b, $c, $d) + $M + $ti;
			if($a > 0x300000000){
					$a = $a - 0x300000000;
				}else
				if($a > 0x200000000){
					$a = $a - 0x200000000;
				}else
				if($a > 0x100000000){
					$a = $a - 0x100000000;
			}
			$a = $this->left($a, $s);
			$a = $a + $b;
			if($a > 0x100000000){
				$a = $a - 0x100000000;
			}
		}
		public function ii(&$a, $b, $c, $d, $M, $s, $ti){
			$a += $this->i($b, $c, $d) + $M + $ti;
			if($a > 0x300000000){
					$a = $a - 0x300000000;
				}else
				if($a > 0x200000000){
					$a = $a - 0x200000000;
				}else
				if($a > 0x100000000){
					$a = $a - 0x100000000;
			}
			$a = $this->left($a, $s);
			$a = $a + $b;
			if($a > 0x100000000){
				$a = $a - 0x100000000;
			}
		}

        // 保存文件
		public function savetemps($input){
			echo '<br>md5encrypt->savetemps();<br>';
			$this->filepath = 'tmp/' . $input;
            echo 'Store in: ' . $this->filepath;
            echo '<br>';
		}
        // 附加填充位
		public function AddPaddingBits(){
			echo '<br>md5encrypt->AddPaddingBits();<br>';
			$this->filelength = filesize($this->filepath);
			echo 'Before add: ' . $this->filelength . 'Bytes<br>';
			$file = fopen($this->filepath,'rb+');
			$a = fread($file,filesize($this->filepath));
			fclose($file);

			$temp = unpack("C*",$a);
			$arr = array();
			foreach ($temp as $key => $value) {
				$arr[$key-1] = $temp[$key];
			}

			if(sizeof($arr) % 64 != 56){
				array_push($arr, 0b10000000);
				while(sizeof($arr) % 64 != 56){
					array_push($arr, 0b00000000);
				}
			}
			$this->file = $arr;
			echo 'Before add: ' . sizeof($this->file) . 'Bytes<br>';
		}
        // 附加长度
		public function AddLength(){
			echo '<br>md5encrypt->AddLength();<br>';
			$len = $this->filelength * 8;
			$a = ($len >> 56);
			$b = ($len >> 48) 	- ($a << 8);
			$c = ($len >> 40) 	- ($a << 16) - ($b << 8);
			$d = ($len >> 32) 	- ($a << 24) - ($b << 16) - ($c << 8);
			$e = ($len >> 24) 	- ($a << 32) - ($b << 24) - ($c << 16) - ($d << 8);
			$f = ($len >> 16) 	- ($a << 40) - ($b << 32) - ($c << 24) - ($d << 16) - ($e << 8);
			$g = ($len >> 8) 	- ($a << 48) - ($b << 40) - ($c << 32) - ($d << 24) - ($e << 16) - ($f << 8);
			$h = $len  			- ($a << 56) - ($b << 48) - ($c << 40) - ($d << 32) - ($e << 24) - ($f << 16) - ($g << 8);
			//array_push($this->file, $a, $b, $c, $d, $e, $f, $g, $h);
			array_push($this->file, $h, $g, $f, $e, $d, $c ,$b, $a);
			//array_push($this->file, $e, $f, $g, $h, $a, $b ,$c, $d);
			echo sizeof($this->file) . 'Bytes<br>';
		}
        // 分组
		public function Merge(){
			echo '<br>md5encrypt->Merge();<br>';
			$temp = array();
			for ($i=0; $i < sizeof($this->file); $i += 4) { 
				array_push($temp, 
				($this->file[$i+3] << 24) + 
				($this->file[$i+2] << 16) +
				($this->file[$i+1] << 8)  +
				$this->file[$i+0]);
			}
			$this->file = $temp;
			echo sizeof($this->file)/16 . 'Rounds<br>';
			// foreach ($this->file as $key => $value) {
			// 	printf('0x%08x <br>', $this->file[$key]);
			// }
		}
        // 计算MD5
		public function Calc(){
            echo '<br>md5encrypt->Merge();<br>';
			$temp = $this->file;
			$result = array(self::A, self::B, self::C, self::D);
			$a = self::A;
			$b = self::B;
			$c = self::C;
			$d = self::D;
			for ($i=0; $i < sizeof($temp); $i += 16) {
				// echo '<br>#Round ' . $i/16 . ' start:<br>';
				$a = $result[0];
				$b = $result[1];
				$c = $result[2];
				$d = $result[3];
				$this->ff($a, $b, $c, $d, $temp[$i + 0], 7, 0xd76aa478);
					$this->ff($d, $a, $b, $c, $temp[$i + 1], 12, 0xe8c7b756);
					$this->ff($c, $d, $a, $b, $temp[$i + 2], 17, 0x242070db);
					$this->ff($b, $c, $d, $a, $temp[$i + 3], 22, 0xc1bdceee);
					$this->ff($a, $b, $c, $d, $temp[$i + 4], 7, 0xf57c0faf);
					$this->ff($d, $a, $b, $c, $temp[$i + 5], 12, 0x4787c62a);
					$this->ff($c, $d, $a, $b, $temp[$i + 6], 17, 0xa8304613);
					$this->ff($b, $c, $d, $a, $temp[$i + 7], 22, 0xfd469501);
					$this->ff($a, $b, $c, $d, $temp[$i + 8], 7, 0x698098d8);
					$this->ff($d, $a, $b, $c, $temp[$i + 9], 12, 0x8b44f7af);
					$this->ff($c, $d, $a, $b, $temp[$i + 10], 17, 0xffff5bb1);
					$this->ff($b, $c, $d, $a, $temp[$i + 11], 22, 0x895cd7be);
					$this->ff($a, $b, $c, $d, $temp[$i + 12], 7, 0x6b901122);
					$this->ff($d, $a, $b, $c, $temp[$i + 13], 12, 0xfd987193);
					$this->ff($c, $d, $a, $b, $temp[$i + 14], 17, 0xa679438e);
				$this->ff($b, $c, $d, $a, $temp[$i + 15], 22, 0x49b40821);
				$this->gg($a, $b, $c, $d, $temp[$i + 1], 5, 0xf61e2562);
					$this->gg($d, $a, $b, $c, $temp[$i + 6], 9, 0xc040b340);
					$this->gg($c, $d, $a, $b, $temp[$i + 11], 14, 0x265e5a51);
					$this->gg($b, $c, $d, $a, $temp[$i + 0], 20, 0xe9b6c7aa);
					$this->gg($a, $b, $c, $d, $temp[$i + 5], 5, 0xd62f105d);
					$this->gg($d, $a, $b, $c, $temp[$i + 10], 9, 0x02441453);
					$this->gg($c, $d, $a, $b, $temp[$i + 15], 14, 0xd8a1e681);
					$this->gg($b, $c, $d, $a, $temp[$i + 4], 20, 0xe7d3fbc8);
					$this->gg($a, $b, $c, $d, $temp[$i + 9], 5, 0x21e1cde6);
					$this->gg($d, $a, $b, $c, $temp[$i + 14], 9, 0xc33707d6);
					$this->gg($c, $d, $a, $b, $temp[$i + 3], 14, 0xf4d50d87);
					$this->gg($b, $c, $d, $a, $temp[$i + 8], 20, 0x455a14ed);
					$this->gg($a, $b, $c, $d, $temp[$i + 13], 5, 0xa9e3e905);
					$this->gg($d, $a, $b, $c, $temp[$i + 2], 9, 0xfcefa3f8);
					$this->gg($c, $d, $a, $b, $temp[$i + 7], 14, 0x676f02d9);
				$this->gg($b, $c, $d, $a, $temp[$i + 12], 20, 0x8d2a4c8a);
				$this->hh($a, $b, $c, $d, $temp[$i + 5], 4, 0xfffa3942);
					$this->hh($d, $a, $b, $c, $temp[$i + 8], 11, 0x8771f681);
					$this->hh($c, $d, $a, $b, $temp[$i + 11], 16, 0x6d9d6122);
					$this->hh($b, $c, $d, $a, $temp[$i + 14], 23, 0xfde5380c);
					$this->hh($a, $b, $c, $d, $temp[$i + 1], 4, 0xa4beea44);
					$this->hh($d, $a, $b, $c, $temp[$i + 4], 11, 0x4bdecfa9);
					$this->hh($c, $d, $a, $b, $temp[$i + 7], 16, 0xf6bb4b60);
					$this->hh($b, $c, $d, $a, $temp[$i + 10], 23, 0xbebfbc70);
					$this->hh($a, $b, $c, $d, $temp[$i + 13], 4, 0x289b7ec6);
					$this->hh($d, $a, $b, $c, $temp[$i + 0], 11, 0xeaa127fa);
					$this->hh($c, $d, $a, $b, $temp[$i + 3], 16, 0xd4ef3085);
					$this->hh($b, $c, $d, $a, $temp[$i + 6], 23, 0x04881d05);
					$this->hh($a, $b, $c, $d, $temp[$i + 9], 4, 0xd9d4d039);
					$this->hh($d, $a, $b, $c, $temp[$i + 12], 11, 0xe6db99e5);
					$this->hh($c, $d, $a, $b, $temp[$i + 15], 16, 0x1fa27cf8);
				$this->hh($b, $c, $d, $a, $temp[$i + 2], 23, 0xc4ac5665);
				$this->ii($a, $b, $c, $d, $temp[$i + 0], 6, 0xf4292244);
					$this->ii($d, $a, $b, $c, $temp[$i + 7], 10, 0x432aff97);
					$this->ii($c, $d, $a, $b, $temp[$i + 14], 15, 0xab9423a7);
					$this->ii($b, $c, $d, $a, $temp[$i + 5], 21, 0xfc93a039);
					$this->ii($a, $b, $c, $d, $temp[$i + 12], 6, 0x655b59c3);
					$this->ii($d, $a, $b, $c, $temp[$i + 3], 10, 0x8f0ccc92);
					$this->ii($c, $d, $a, $b, $temp[$i + 10], 15, 0xffeff47d);
					$this->ii($b, $c, $d, $a, $temp[$i + 1], 21, 0x85845dd1);
					$this->ii($a, $b, $c, $d, $temp[$i + 8], 6, 0x6fa87e4f);
					$this->ii($d, $a, $b, $c, $temp[$i + 15], 10, 0xfe2ce6e0);
					$this->ii($c, $d, $a, $b, $temp[$i + 6], 15, 0xa3014314);
					$this->ii($b, $c, $d, $a, $temp[$i + 13], 21, 0x4e0811a1);
					$this->ii($a, $b, $c, $d, $temp[$i + 4], 6, 0xf7537e82);
					$this->ii($d, $a, $b, $c, $temp[$i + 11], 10, 0xbd3af235);
					$this->ii($c, $d, $a, $b, $temp[$i + 2], 15, 0x2ad7d2bb);
				$this->ii($b, $c, $d, $a, $temp[$i + 9], 21, 0xeb86d391);
				$result[0] = $result[0] + $a;
				$result[1] = $result[1] + $b;
				$result[2] = $result[2] + $c;
				$result[3] = $result[3] + $d;
				if($result[0] > 0x100000000){
					$result[0] = $result[0] - 0x100000000;
				}
				if($result[1] > 0x100000000){
					$result[1] = $result[1] - 0x100000000;
				}
				if($result[2] > 0x100000000){
					$result[2] = $result[2] - 0x100000000;
				}
				if($result[3] > 0x100000000){
					$result[3] = $result[3] - 0x100000000;
				}
				// echo '<br>#Round ' . $i/16 . ' end<br>';
			}
			printf(
				'%08x%08x%08x%08x <br>', 
				$this->fix($result[0]), 
				$this->fix($result[1]), 
				$this->fix($result[2]), 
				$this->fix($result[3])
			);
		}
	}
?>