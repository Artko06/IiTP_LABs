import Data.SortedMap

forLoop : Nat -> IO ()
forLoop Z = putStrLn "Done"
forLoop (S k) = do
    putStrLn ("Iteration: " ++ show k)
    forLoop k

main : IO ()
main = do
    let bigInt : Integer = 12345678901234567890
    let bVal : Bits8 = 255
    let dict : SortedMap String Int = insert "key" 42 empty
    forLoop 3 


-- idris -p contrib file3.idr -o file3 && ./file3