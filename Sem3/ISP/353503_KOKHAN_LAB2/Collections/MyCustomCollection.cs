using _353503_KOKHAN_LAB2.Interfaces;

namespace _353503_KOKHAN_LAB2.Collections
{
    public class MyCustomCollection<T> : ICustomCollection<T>, IEnumerable<T>
    {
        private class Node
        {
            public T Value { get; set; }
            public Node? Next { get; set; }
            public Node(T value)
            {
                Value = value;
                Next = null;
            }
        }

        private Node? head;
        private Node? current;
        private int count;

        public MyCustomCollection()
        {
            head = null;
            current = null;
            count = 0;
        }

        public T this[int index]
        {
            get
            {
                if (index < 0 || index >= count) throw new IndexOutOfRangeException();
                var tempNode = head;
                for (int i = 0; i < index; ++i)
                {
                    tempNode = tempNode!.Next;
                }
                return tempNode!.Value;
            }
            set
            {
                if (index < 0 || index >= count) throw new IndexOutOfRangeException();
                var tempNode = head;
                for (int i = 0; i < index; ++i)
                {
                    tempNode = tempNode!.Next;
                }
                tempNode!.Value = value;
            }
        }

        public int Count{ get { return count;}}

        public void Add(T item)
        {
            var newNode = new Node(item);
            if (head == null)
            {
                head = newNode;
                current = head;
            }
            else
            {
                var node = head;
                while (node.Next != null)
                {
                    node = node.Next;
                }
                node.Next = newNode;
            }
            ++count;
        }

        public void Remove(T item)
        {
            if (head == null) throw new Exception("Cannot remove");

            if (head.Value!.Equals(item))
            {
                if (current == head)
                {
                    current = head.Next;
                }
                head = head.Next;
                --count;
                return;
            }

            var node = head;
            while (node.Next != null && !node.Next.Value!.Equals(item))
            {
                node = node.Next;
            }

            if (node.Next == null) throw new Exception("Cannot find item in list");

            if (current == node.Next)
            {
                current = current.Next;
            }

            node.Next = node.Next.Next;
            --count;
        }

        public T RemoveCurrent()
        {
            if (current == null) throw new InvalidOperationException("Current == null");

            var node = head;
            if (current == head)
            {
                head = head.Next;
                --count;
                return current.Value;
            }

            while (node!.Next != null && node.Next != current)
            {
                node = node.Next;
            }

            if (node.Next == null) throw new InvalidOperationException("Current is not found");

            node.Next = current.Next;
            var data = current.Value;
            current = current.Next;
            --count;

            return data;
        }

        public T Current()
        {
            if (current == null) throw new InvalidOperationException("Current is null");
            return current.Value;
        }

        public void Reset()
        {
            current = head;
        }

        public void Next()
        {
            if (current != null)
            {
                current = current.Next;
            }
        }

        public IEnumerator<T> GetEnumerator()
        {
            var node = head;
            while (node != null)
            {
                yield return node.Value;
                node = node.Next;
            }
        }

        System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }
    }
}