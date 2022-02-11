hdfs dfs -test -d main && hdfs dfs -rm -r main
hdfs dfs -mkdir main
hdfs dfs -copyFromLocal input.txt main/
hdfs dfs -cat main/input.txt

hadoop jar ../hadoop-streaming-3.3.1.jar \
-input main/input.txt \
-output main/output \
-mapper mapper.py \
-reducer reducer.py

hdfs dfs -cat main/output/part-00000
