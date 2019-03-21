using UnityEngine;
using UnityEditor;
using System.IO;

public class CustomFBXImporter : MonoBehaviour
{
    public string binaryPath = "";

    public string asciiPath = "";

    struct Vertex
    {
        float bajs;
    }

    public void ReadBinaryFile()
    {

        TextAsset asset = Resources.Load(binaryPath) as TextAsset;
        Stream s = new MemoryStream(asset.bytes);
        BinaryReader br = new BinaryReader(s);

        var formatter = new System.Runtime.Serialization.Formatters.Binary.BinaryFormatter();
        //br.ReadBytes(sizeof(Vertex));
    }

    public void ReadASCIIFile()
    {
        string path = asciiPath;

        //Read the text from directly from the test.txt file
        StreamReader reader = new StreamReader(path);
        Debug.Log(reader.ReadToEnd());
        reader.Close();
    }

    [CustomEditor(typeof(CustomFBXImporter))]
    public class ObjectBuilderEditor : Editor
    {
        public override void OnInspectorGUI()
        {
            DrawDefaultInspector();

            CustomFBXImporter myScript = (CustomFBXImporter)target;
            if (GUILayout.Button("Import Binary"))
            {
                myScript.ReadBinaryFile();
            }

            if (GUILayout.Button("Import ASCII"))
            {
                myScript.ReadASCIIFile();
            }
        }
    }
}
