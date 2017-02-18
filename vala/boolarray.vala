/*
 * boolarray.vala
 *
 * Copyright (C) 2017 - Pavel Nikitin
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * Class BoolArray implements an array of booleans. Each value occupies one bit.
 */
class BoolArray : GLib.Object
{
  public uint64[] array = null;
  public int length { construct; get; }

  public BoolArray () {}

  public BoolArray.size (int length)
  {
    Object (length: length);
  }

  public BoolArray.copy (BoolArray source)
  {
    Object (length: source.length);
    for (var i = 0, n = source.array.length; i < n; i++)
      array[i] = source.array[i];
  }

  construct
  {
    if (length > 0)
      {
        int size = length >> BIT_SHIFT;
        if ((length & AND_MASK) != 0)
          ++size;
        array = new uint64[size];
      }
  }

  public new bool get (int index)
  {
    return ((array[index >> BIT_SHIFT] >> 
      (index & AND_MASK)) & 1) == 0 ? false : true;
  }

  public new void set (int index, bool value)
  {
    if (value)
      array[index >> BIT_SHIFT] |=    1ul << (index & AND_MASK);
    else
      array[index >> BIT_SHIFT] &= ~ (1ul << (index & AND_MASK));
  }

  public void xor (int index, bool value)
  {
    if (value)
      array[index >> BIT_SHIFT] ^= 1ul << (index & AND_MASK);
  }
 
  public int count()
  {
    int    count = 0, len = array.length, i;
    uint64 word;

    for (i = 0; i < len; i++)
      {
        word = array[i];
        while (word != 0)
          {
            word &= (word - 1);
            count++;
          }
      }

    return count;
  }

  public void clear ()
  {
    int i, len = array.length;
    for (i = 0; i < len; i++)
      array[i] = 0;
  }

  public new string to_string ()
  {
    string result = "";
    for (int i = 0; i < length; i++)
      {
        if (this[i])
          result += "1";
        else
          result += "0";
      }
    return result;
  }

  private const int BIT_SHIFT = 6;
  private const int AND_MASK = 63;
}
