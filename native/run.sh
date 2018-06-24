logfile=`mktemp`
temperature=`mktemp`
humidity=`mktemp`

scp raspberrypi:log.txt $logfile 

cat $logfile | grep temperature | awk -f ./parsedlm.awk > $temperature 
# cat $logfile | grep humidity    | awk -f ./parsedlm.awk > $humidity 

julia -p $(nproc) ./plotthis.jl $temperature
# julia -p $(nproc) ./plotthis.jl $temperature $humidity
