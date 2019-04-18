
## collect
#### Usage
collect [-i infile] [-o outfile] [-c summary_field] 
          [-d "input_field_sep"] [-O "output_field_sep"] [-S "summary_sep"]
          [-D](duplicates) [-U](unsorted) [-E] (empty fields ok)

-c field  - the field to summarize counted from 1
-d chars  - the character(s) delimiting fields default ":"
-O chars  - string separating fields on output default ":"
-S chars  - string separating summarized fields default ","

Input must be sorted as only adjacent lines are summarized.


