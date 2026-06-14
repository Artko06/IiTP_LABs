numbersList : List Int
numbersList = [10, 20, 30]

checkValue : Int -> Double -> String
checkValue x d = 
    let n : Nat = 5 in
    if cast x > n && d > 0.0 
       then "Greater than 5 and Double is positive" 
       else "Condition not met"

main : IO ()
main = do
    let result = checkValue 10 3.14159 
    putStrLn result
   
    
-- idris file1.idr -o file1 && ./file1