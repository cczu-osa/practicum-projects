$plaintext = (Get-Item -Path ".\" -Verbose).FullName + '\100MB.rand'
$cpuChiper = (Get-Item -Path ".\" -Verbose).FullName + '\encrypted-cpu'
$gpuChiper = (Get-Item -Path ".\" -Verbose).FullName + '\encrypted-gpu'
$cpuNChiper = (Get-Item -Path ".\" -Verbose).FullName + '\decrypted-cpu'
$gpuNChiper = (Get-Item -Path ".\" -Verbose).FullName + '\decrypted-gpu'

"[Prepare] Create a random file to encrypt..."

$fileContent = New-Object Byte[] 104857600
(New-Object Random).NextBytes($fileContent)

"[Prepare] Writing data to filesystem..."
[IO.File]::WriteAllBytes($plaintext, $fileContent)
"[TEST][CPU] Done, SHA256 of result is: {0}" -f $(Get-FileHash $plaintext -Algorithm SHA256).Hash

"[TEST][CPU] Using CPU to encrypt 100MB file with 8 threads..."
$(.\AES-CPU-MT.exe -m encrypt -i $plaintext -o $cpuChiper -k 0123456789abcdeffedcba9876543210 -t 8) | out-null
"[TEST][CPU] Done, SHA256 of result is: {0}" -f $(Get-FileHash $cpuChiper -Algorithm SHA256).Hash

"[TEST][CPU] Using CPU to decrypt 100MB file with 8 threads..."
$(.\AES-CPU-MT.exe -m decrypt -i $cpuChiper -o $cpuNChiper -k 0123456789abcdeffedcba9876543210 -t 8) | out-null
"[TEST][CPU] Done, SHA256 of result is: {0}" -f $(Get-FileHash $cpuNChiper -Algorithm SHA256).Hash

"[TEST][GPU] Using GPU to encrypt 100MB file..."
$(.\AES-GPU.exe -m encrypt -i $plaintext -o $gpuChiper -k 0123456789abcdeffedcba9876543210) | out-null
"[TEST][GPU] Done, SHA256 of result is: {0}" -f $(Get-FileHash $gpuChiper -Algorithm SHA256).Hash

"[TEST][GPU] Using GPU to decrypt 100MB file..."
$(.\AES-GPU.exe -m decrypt -i $gpuChiper -o $gpuNChiper -k 0123456789abcdeffedcba9876543210) | out-null
"[TEST][GPU] Done, SHA256 of result is: {0}" -f $(Get-FileHash $gpuNChiper -Algorithm SHA256).Hash
