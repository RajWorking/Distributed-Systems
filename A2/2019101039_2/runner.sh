hdfs dfs -mkdir $3/input/
hdfs dfs -copyFromLocal $2 $3/input/
hadoop jar $1 \
-input $3/input/ \
-output $4 \
-mapper $5/mapper.py \
-reducer $6/reducer.py
