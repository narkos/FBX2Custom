using System;
using System.IO;
using System.Text;

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;

using System.Runtime.InteropServices;

public class ImportDataTypes : MonoBehaviour
{
    public struct Scene
    {
        public Header header;
        public Body body;

        public struct Header
        {
            public int transformCount;
            public int meshCount;  //Not rly needed just check vector.size
            public int materialCount;
            public int lightCount;
            public int cameraCount;
        }

        public struct Body
        {
            public Transform[] transforms;
            public Mesh[] meshes;
            public Light[] lights;
        }

        public void Read(ref BinaryReader br)
        {
            header.transformCount = br.ReadInt32();
            header.meshCount = br.ReadInt32();
            header.materialCount = br.ReadInt32();
            header.lightCount = br.ReadInt32();
            header.cameraCount = br.ReadInt32();

            body.transforms = new Transform[header.transformCount];
            for (int i = 0; i < body.transforms.Length; i++)
            {
                Transform newObject = new Transform();
                newObject.Read(ref br);
                body.transforms[i] = newObject;
            }

            // Read meshes.
            body.meshes = new Mesh[header.meshCount];
            for (int i = 0; i < header.meshCount; i++)
            {
                Mesh newObject = new Mesh();
                newObject.Read(ref br);
                body.meshes[i] = newObject;
            }

            // Read lights.
            body.lights = new Light[header.lightCount];
            for (int i = 0; i < header.lightCount; i++)
            {
                Light newObject = new Light();
                newObject.Read(ref br);
                body.lights[i] = newObject;
            }
        }

        #region On scene existing
        public Transform GetTransform(string transformName)
        {
            for (int i = 0; i < body.transforms.Length; i++)
            {
                if (body.transforms[i].name.text == transformName)
                {
                    return body.transforms[i];
                }
            }

            print("Couldn't find transform by name: " + transformName);
            return new Transform();
        }
        #endregion
    }

    public struct Mesh
    {
        public Header header;
        public Body body;

        public void Read(ref BinaryReader br)
        {
            header.transformName.Read(ref br);
            header.nrVertices = br.ReadInt32();

            body.vertices = new Vertex[header.nrVertices];

            for (int i = 0; i < header.nrVertices; i++)
            {
                Vertex vertex = new Vertex();
                vertex.Read(ref br);

                body.vertices[i] = vertex;
            }
        }

        public struct Header
        {
            public StringObj transformName;

            //public Material material;
            public int nrVertices;
        }

        public struct Body
        {
            public Vertex[] vertices;
        }
    }

    public struct Light
    {
        public Transform transform;
        public double range;
        public double intensity;

        public void Read(ref BinaryReader br)
        {
            transform.Read(ref br);
            range = br.ReadDouble();
            intensity = br.ReadDouble();
        }
    }

    public struct Transform
    {
        public StringObj name;
        public Vec3 position;
        public Vec3 rotation;
        public Vec3 scale;

        public void Read(ref BinaryReader br)
        {
            name.Read(ref br);
            position.Read(ref br);
            rotation.Read(ref br);
            scale.Read(ref br);
        }
    }

    public struct Vertex
    {
        public Vec3 position;
        public Vec3 normal;
        public Vec2 uv;

        public void Read(ref BinaryReader br)
        {
            position.Read(ref br);
            normal.Read(ref br);
            uv.Read(ref br);
            ValidateValues();
        }

        private void ValidateValues()
        {
            UnityEngine.Vector3 normalVector = new UnityEngine.Vector3((float)normal.x, (float)normal.y, (float)normal.z);

            if (normalVector.magnitude > 1)
            {
                print("Invalid vertex, weird normal: " + normalVector.ToString());
            }

            if (System.Math.Abs(uv.x) > 1f || System.Math.Abs(uv.y) > 1f)
            {
                print("Invalid vertex, weird UV: (" + uv.x + "," + uv.y + ")");
            }
        }
    }

    public struct Vec4
    {
        public double x;
        public double y;
        public double z;
        public double w;

        public void Read(ref BinaryReader br)
        {
            x = br.ReadDouble();
            y = br.ReadDouble();
            z = br.ReadDouble();
            w = br.ReadDouble();
        }

        public UnityEngine.Vector4 Create()
        {
            return new UnityEngine.Vector4((float)x, (float)y, (float)z, (float)w);
        }
    }

    public struct Vec3
    {
        public double x;
        public double y;
        public double z;

        public void Read(ref BinaryReader br)
        {
            x = br.ReadDouble();
            y = br.ReadDouble();
            z = br.ReadDouble();
        }

        public UnityEngine.Vector3 Create()
        {
            return new UnityEngine.Vector3((float)x, (float)y, (float)z);
        }
    }

    public struct Vec2
    {
        public double x;
        public double y;

        public void Read(ref BinaryReader br)
        {
            x = br.ReadDouble();
            y = br.ReadDouble();
        }

        public UnityEngine.Vector2 Create()
        {
            return new UnityEngine.Vector2((float)x, (float)y);
        }
    }

    public struct StringObj
    {
        public int textLength;
        public string text;

        public void Read(ref BinaryReader br)
        {
            textLength = br.ReadInt32();
            text = new string(br.ReadChars(textLength));
        }
    }
}
